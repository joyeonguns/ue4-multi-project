// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Engine/GameEngine.h"
#include "GameFramework/SpringArmComponent.h"
#include "AnimInstance_Player.h"
#include <Components/WidgetComponent.h>
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyGunActor.h"
#include "MyPlayerController.h"
#include "UI_FloatingHP.h"
#include "Character_State_Component.h"
#include "Charater_SKill_Component.h"
// Sets default values

AThirdPersonCharacter::AThirdPersonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	RootComponent = GetCapsuleComponent();

	GetMesh()->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform); //AttachTo(RootComponent);

	SetReplicates(true);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// FP 
	FP_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FP_Camera"));
	FP_Camera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform); //AttachTo(RootComponent);
	FP_Camera->bUsePawnControlRotation = true;

	FP_Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Arms"));
	FP_Arms->AttachToComponent(FP_Camera, FAttachmentTransformRules::KeepRelativeTransform); //AttachTo(FP_Camera);

	ShotPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ShotPoint"));

	ScopeCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("ScopeCamera"));
	ScopeCamera->AttachToComponent(FP_Camera, FAttachmentTransformRules::KeepRelativeTransform); //AttachTo(FP_Camera);

	// 캐릭터 스텟 컴포넌트
	Character_State_Component = CreateDefaultSubobject<UCharacter_State_Component>(TEXT("StateComponent"));
	Character_State_Component->player = this;
	BindFloatingHP();

	// 캐릭터 스킬 컴포넌트
	Character_Skill_Component = CreateDefaultSubobject<UCharater_SKill_Component>(TEXT("Skill_Component"));
	Character_Skill_Component->player = this;

	//// Attribute
	//AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>("AbilitySystemComponent");
	//AbilitySystemComponent->SetIsReplicated(true);
	//AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	//Attributes = CreateDefaultSubobject<UGASAttributeSet>("Attributes");
}

void AThirdPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// BindAction
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AThirdPersonCharacter::InputJump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AThirdPersonCharacter::InputRun);
	PlayerInputComponent->BindAction("Sit", IE_Pressed, this, &AThirdPersonCharacter::InputSit);

	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AThirdPersonCharacter::InputInteraction);
	PlayerInputComponent->BindAction("ThrowGun", IE_Pressed, this, &AThirdPersonCharacter::ThrowGun);
	PlayerInputComponent->BindAction("ChangeCamera", IE_Pressed, this, &AThirdPersonCharacter::InputZoom);
	PlayerInputComponent->BindAction("R", IE_Pressed, this, &AThirdPersonCharacter::InputReload);

	PlayerInputComponent->BindAction("skill1", IE_Pressed, this, &AThirdPersonCharacter::InputSkill0);
	PlayerInputComponent->BindAction("skill2", IE_Pressed, this, &AThirdPersonCharacter::InputSkill1);

	PlayerInputComponent->BindAction("SelectGun_0", IE_Pressed, this, &AThirdPersonCharacter::InputSelectGun_0);
	PlayerInputComponent->BindAction("SelectGun_1", IE_Pressed, this, &AThirdPersonCharacter::InputSelectGun_1);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AThirdPersonCharacter::InputFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AThirdPersonCharacter::InputFireEnd);


	// BindAxis
	PlayerInputComponent->BindAxis("MoveForward", this, &AThirdPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AThirdPersonCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnRate", this, &AThirdPersonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &AThirdPersonCharacter::LookUpAtRate);

}

void AThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Floating HP
	auto hp_ui = Cast<UUI_FloatingHP>(floatingHP->GetUserWidgetObject());
	if (hp_ui) {
		//hp_ui->BindCharacterStat(this);
	}

	// 조준경 텍스쳐
	SetScopeTarget();

	anim = Cast<UAnimInstance_Player>(GetMesh()->GetAnimInstance());
	if (anim) {
		anim->OnDelicate_StartChangeGun.AddLambda([this]() -> void {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("InputSelectGun"));
			bChangeGun = true;
			});
		anim->OnDelicate_EndChangeGun.AddLambda([this]() -> void {
			bChangeGun = false;
			});
	}

	// 플레이어 컨트롤러 - 컴포넌트 바인딩
	plController = Cast<AMyPlayerController>(GetController());
	if (plController) {
		plController->BindStateComponent(Character_State_Component);
		plController->BindSkillComponent(Character_Skill_Component);
	}

	// 파쿠르 
	VaultStartLoc = GetActorLocation();
	VaultEndLoc = GetActorLocation() + FVector(300.f, 300.f, 300.f);

	// 카메라 초기 설정
	BeginSetting();

	// 총 발사 위치
	ShotPoint->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Gun_R"));

	if (HasAuthority()) {
		SetSetting();
	}
	else {
		SetSetting();
	}

	// 체력
	Character_State_Component->SetHp(100);
	Character_State_Component->SetShield(100);


}

void AThirdPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 캐릭터 회전값
	SetRotOnServer(GetActorRotation());

	// 파쿠르
	CheckVaulting();
	Vaulting(DeltaTime);



	if (HasAuthority()) {
		SearchInteractionObj();
		SearchHipShotPointDirection();
		SearchAimShotPointDirection();

		SearchAimOnServer(FP_Camera->GetComponentTransform());
		SetFireSyncTransformOnServer(ShotPoint->GetComponentTransform());
	}
	else {
		SearchAimOnServer(FP_Camera->GetComponentTransform());
		SetFireSyncTransformOnServer(ShotPoint->GetComponentTransform());
	}

	if (GetCharacterMovement()->IsFalling()) {
		if (plController) {
			plController->Falling();
		}
	}
	else {
		if (plController) {
			plController->EndFalling();
		}
	}

	SetPitchYawOnServer(DeltaTime);

	Zooming(DeltaTime);

	if (anim->Montage_IsPlaying(Anim_Reload) || anim->Montage_IsPlaying(Anim_Zoom) || anim->Montage_IsPlaying(Anim_ChangeGun)) {

		OnlyBodyAnim = true;
	}
	else {

		OnlyBodyAnim = false;
	}


	if (EquipedGuns[curGun].IsValid()) {
		FString armo;
		armo = FString::FromInt(EquipedGuns[curGun]->Get_C_Armo());
		if (plController)
			plController->GetCrosshairUI()->UpdateArmo(armo, curGun);
	}

}

void AThirdPersonCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AThirdPersonCharacter, EquipedGuns);
	DOREPLIFETIME(AThirdPersonCharacter, SearchGun);
	DOREPLIFETIME(AThirdPersonCharacter, Aim_Pitch);
	DOREPLIFETIME(AThirdPersonCharacter, Aim_Yaw);
	DOREPLIFETIME(AThirdPersonCharacter, Zoom);
	DOREPLIFETIME(AThirdPersonCharacter, OnlyBodyAnim);
	DOREPLIFETIME(AThirdPersonCharacter, curGun);
	DOREPLIFETIME(AThirdPersonCharacter, MyTeam);

}

void AThirdPersonCharacter::OnResetVR()
{
	// If VaultTutorial is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in VaultTutorial.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AThirdPersonCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AThirdPersonCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AThirdPersonCharacter::InputRun()
{
	if (bRun == false) {
		GetCharacterMovement()->MaxWalkSpeed = 750.f;
		SwitchRunOnServer();
		bRun = true;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = 350.f;
		SwitchWalkOnServer();
		bRun = false;
	}
}

void AThirdPersonCharacter::InputInteraction()
{
	//asdf();
	if (SearchGun != nullptr) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT(" try Equip "));
		//EquipGunOnServer();
		Equip2();
		//Equip2OnServer();
		//Equip2OnMulti();


	}
	else {
		//ThrowGunOnServer();
	}

}

void AThirdPersonCharacter::SwitchRunOnServer_Implementation()
{
	SwitchRunOnMulti();
}

void AThirdPersonCharacter::SwitchWalkOnServer_Implementation()
{
	SwitchWalkOnMulti();
}


void AThirdPersonCharacter::SwitchRunOnMulti_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 700.f;
}

void AThirdPersonCharacter::SwitchWalkOnMulti_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Walk"));
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Walk, %f"), GetCharacterMovement()->MaxWalkSpeed));
}


void AThirdPersonCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AThirdPersonCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AThirdPersonCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && !bVaulting)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AThirdPersonCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


void AThirdPersonCharacter::InputJump()
{
	if (bCanVault) {
		bVaulting = true;
		bCanVault = false;
		if (Anim_DeepVault) {
			PlayAnimMontageOnServer(Anim_DeepVault);
			VaultEndLoc = VaultTargetLoc;
			VaultStartLoc = GetActorLocation();
		}
	}
	else if (!GetCharacterMovement()->IsFalling()) {
		PlayAnimMontageOnServer(Anim_Jump);
		Jump();
	}
}

void AThirdPersonCharacter::Vaulting(float DeltaTime)
{
	if (bVaulting) {

		VaultTime += DeltaTime;
		SetActorLocation(FMath::Lerp(VaultStartLoc, VaultEndLoc, VaultTime / 1.0f));
		SetActorLocationOnServer(FMath::Lerp(VaultStartLoc, VaultEndLoc, VaultTime / 1.0f));


		if (VaultTime >= 1.0) {
			bVaulting = false;
			VaultTime = 0.0f;
		}
	}

}

void AThirdPersonCharacter::CheckVaulting()
{
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (EquipedGuns[0].IsValid()) {
		Params.AddIgnoredActor(EquipedGuns[0].Get());
	}
	if (EquipedGuns[1].IsValid()) {
		Params.AddIgnoredActor(EquipedGuns[1].Get());
	}

	FVector ForwardVec = GetActorLocation() + GetActorForwardVector() * 80.f;
	float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector Start = ForwardVec + FVector(0, 0, CapsuleHalfHeight + 10);
	FVector End = ForwardVec;

	bool bResult = GetWorld()->LineTraceSingleByChannel(
		OUT HitResult,
		Start, End,
		ECollisionChannel::ECC_GameTraceChannel2,
		Params
	);

	FColor DebugColor;
	if (bResult && HitResult.Location.Z < Start.Z) {
		DebugColor = FColor::Green;
		VaultTargetLoc = HitResult.Location + FVector(0, 0, CapsuleHalfHeight);
	}
	else {
		DebugColor = FColor::Red;
	}

	DrawDebugLine(GetWorld(), Start, End, DebugColor, false, 1.f);

	bCanVault = (bResult && HitResult.Location.Z < Start.Z);
}

void AThirdPersonCharacter::InputSit()
{
	if (GetVelocity().Size() > 500.f) {
		Sliding();
	}
}

void AThirdPersonCharacter::Sliding()
{
	PlayAnimMontageOnServer(Anim_Sliding);
}

void AThirdPersonCharacter::Hit()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Hit Projectile : ") + GetName());
}

float AThirdPersonCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Character_State_Component->TakeDamage(Damage);
	/*CurrentHP -= Damage;

	if (CurrentHP <= 0) {
		CurrentHP = 0;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT(" DIE : ") + GetName());
		PlayAnimMontageOnServer(Anim_Death);
	}

	OnUpdateHP.Broadcast();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, GetName() + FString::Printf(TEXT(" : HP : %d"), CurrentHP));
	*/

	return Damage;
}

void AThirdPersonCharacter::ChangeCamera()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("ChangeCamera ")));
	if (Zoom) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Zoom")));
		// TP 전환
		FP_Camera->SetActive(false);
		FollowCamera->SetActive(true);
		FP_Arms->SetVisibility(false);
		GetMesh()->SetVisibility(true);
		if (FP_Gun != nullptr)	FP_Gun->SetVisibility(false);
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;

		HiddeFirstPersonOnServer();

		if (EquipedGuns[curGun].IsValid())	EquipedGuns[curGun]->GetSK_Mesh()->SetVisibility(true);

		//bFirstPersonMode = false;

		_playerState = EStateEnum::Hipfire;

		SetZoom(false);
		SetZoomOnServer(false);

	}
	else {
		if (EquipedGuns[curGun].IsValid() && EquipedGuns[curGun]->Get_C_Armo() > 0) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("!Zoom")));
			// FP 전환
			//GetMesh()->SetVisibility(false);
			FP_Camera->SetActive(true);
			FollowCamera->SetActive(false);
			FP_Arms->SetVisibility(true);
			if (FP_Gun != nullptr)	FP_Gun->SetVisibility(true);
			bUseControllerRotationYaw = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;

			if (EquipedGuns[curGun].IsValid())	EquipedGuns[curGun]->GetSK_Mesh()->SetVisibility(false);

			//bFirstPersonMode = true;

			_playerState = EStateEnum::Zoom;

			SetZoom(true);
			SetZoomOnServer(true);

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("ChangeCamera : first")));
		}
	}
}

void AThirdPersonCharacter::SearchInteractionObj()
{
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	if (EquipedGuns[0].IsValid()) {
		Params.AddIgnoredActor(EquipedGuns[0].Get());
	}
	if (EquipedGuns[1].IsValid()) {
		Params.AddIgnoredActor(EquipedGuns[1].Get());
	}

	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + FollowCamera->GetForwardVector() * 300.0f;

	bool bResult = GetWorld()->LineTraceSingleByChannel(
		OUT HitResult,
		Start, End,
		ECollisionChannel::ECC_GameTraceChannel2,
		Params
	);

	FColor DebugColor;
	if (bResult) {
		DebugColor = FColor::Green;
	}
	else {
		DebugColor = FColor::Red;
	}

	DrawDebugLine(GetWorld(), Start, End, DebugColor, false, 1.f);

	if (bResult) {
		auto weapon = Cast<AMyGunActor>(HitResult.Actor);
		if (weapon) {
			if (weapon && weapon->Get_Equiped() == false) {
				SearchGunOnServer(weapon);
			}
			else {
				SearchGunOnServer(nullptr);
			}
		}
	}
	else {
		SearchGunOnServer(nullptr);
	}
}

void AThirdPersonCharacter::SearchHipShotPointDirection()
{
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	if (EquipedGuns[0].IsValid()) {
		Params.AddIgnoredActor(EquipedGuns[0].Get());
	}
	if (EquipedGuns[1].IsValid()) {
		Params.AddIgnoredActor(EquipedGuns[1].Get());
	}

	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + FollowCamera->GetForwardVector() * 10000.0f;

	bool bResult = GetWorld()->LineTraceSingleByChannel(
		OUT HitResult,
		Start, End,
		ECollisionChannel::ECC_GameTraceChannel2,
		Params
	);

	if (bResult) {
		HipShotPoint = HitResult.Location;
	}
	else {
		HipShotPoint = End;
	}
	OnRep_UpdatedAimDirection();

	FColor DebugColor = FColor::Blue;

	DrawDebugLine(GetWorld(), Start, End, DebugColor, false, 1.f);
}

void AThirdPersonCharacter::SearchAimShotPointDirection()
{
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	if (EquipedGuns[curGun].IsValid()) {
		Params.AddIgnoredActor(EquipedGuns[curGun].Get());
	}

	FVector Start = FP_Camera->GetComponentLocation();
	FVector End = Start + FP_Camera->GetForwardVector() * 10000.0f;

	bool bResult = GetWorld()->LineTraceSingleByChannel(
		OUT HitResult,
		Start, End,
		ECollisionChannel::ECC_GameTraceChannel2,
		Params
	);

	if (bResult) {
		AimShotPoint = HitResult.Location;
	}
	else {
		AimShotPoint = End;
	}

	FColor DebugColor = FColor::Green;

	DrawDebugLine(GetWorld(), Start, End, DebugColor, false, 1.f);


}

void AThirdPersonCharacter::SearchAimOnServer_Implementation(FTransform tp)
{
	CamTransform = tp;
	if (IsLocallyControlled()) return;

	FP_Camera->SetWorldTransform(CamTransform);
}

void AThirdPersonCharacter::SearchAimOnMulti_Implementation(FTransform tp)
{

}

void AThirdPersonCharacter::InputFire()
{
	if (bChangeGun == true) return;
	Fire();
	/*if (EquipedGuns[curGun].IsValid() && EquipedGuns[curGun]->GunData.GunType == "Sniper") return;

	GetWorldTimerManager().SetTimer(TH_Fire, this, &AThirdPersonCharacter::Fire, 0.01f, true);*/

}

void AThirdPersonCharacter::InputFireEnd()
{
	FireEndOnServer();
	//GetWorldTimerManager().ClearTimer(TH_Fire);
}

void AThirdPersonCharacter::FireEndOnServer_Implementation()
{
	FireEndOnMulti();
}

void AThirdPersonCharacter::FireEndOnMulti_Implementation()
{
	if (EquipedGuns[curGun].IsValid()) {
		EquipedGuns[curGun]->EndFire();
	}
}

void AThirdPersonCharacter::Fire()
{
	InputFireOnServer();

	// 재장전
	ZoomOff();
}

void AThirdPersonCharacter::SetPitchYaw(float DeltaTime)
{
	FRotator CurrentRot = FRotator(Aim_Pitch, Aim_Yaw, 0.0f);
	FRotator TargetRot = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation());
	FRotator rot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, 15.f);
	Aim_Yaw = UKismetMathLibrary::ClampAngle(rot.Yaw, -90.f, 90.f);
	Aim_Pitch = UKismetMathLibrary::ClampAngle(rot.Pitch, -90.f, 90.f);

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("pitch, %f"), Aim_Pitch));
}

float AThirdPersonCharacter::Get_Pitch()
{
	return Aim_Pitch;
}

float AThirdPersonCharacter::Get_Yaw()
{
	return Aim_Yaw;
}

void AThirdPersonCharacter::BeginSetting()
{
	// FP 전환
			//GetMesh()->SetVisibility(false);
	FP_Camera->SetActive(true);
	FollowCamera->SetActive(false);
	FP_Arms->SetVisibility(true);
	if (FP_Gun != nullptr)	FP_Gun->SetVisibility(true);
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	if (EquipedGuns[curGun].IsValid())	EquipedGuns[curGun]->GetSK_Mesh()->SetVisibility(false);

	bFirstPersonMode = true;

	_playerState = EStateEnum::Zoom;

	SetZoom(true);
	SetZoomOnServer(true);


	// TP 전환
	FP_Camera->SetActive(false);
	FollowCamera->SetActive(true);
	FP_Arms->SetVisibility(false);
	GetMesh()->SetVisibility(true);
	if (FP_Gun != nullptr)	FP_Gun->SetVisibility(false);
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	HiddeFirstPersonOnServer();

	if (EquipedGuns[curGun].IsValid())	EquipedGuns[curGun]->GetSK_Mesh()->SetVisibility(true);

	bFirstPersonMode = false;

	_playerState = EStateEnum::Hipfire;

	SetZoom(false);
	SetZoomOnServer(false);
}


void AThirdPersonCharacter::SetGravityScaleOnServer_Implementation(float _scale)
{
	SetGravityScaleOnClient(_scale);
}

void AThirdPersonCharacter::SetGravityScaleOnClient_Implementation(float _scale)
{
	GetCharacterMovement()->GravityScale = _scale;
}

void AThirdPersonCharacter::PlayAnimMontageOnServer_Implementation(UAnimMontage* _AnimMontage)
{
	PlayAnimMontageOnClinet(_AnimMontage);
}

void AThirdPersonCharacter::SetActorLocationOnServer_Implementation(FVector newLocation)
{
	//FVector dir = (newLocation - GetActorLocation()).GetSafeNormal();

	//GetCharacterMovement()->AddForce(dir * 300000.0f);
	SetActorLocationOnMulti(newLocation);
	SetActorLocation(newLocation);
}

void AThirdPersonCharacter::SetActorLocationOnMulti_Implementation(FVector newLocation)
{
	SetActorLocation(newLocation);
}


void AThirdPersonCharacter::PlayAnimMontageOnClinet_Implementation(UAnimMontage* _AnimMontage)
{
	PlayAnimMontage(_AnimMontage);
}

void AThirdPersonCharacter::HiddeFirstPersonOnServer_Implementation()
{
	FP_Arms->SetVisibility(false);
	if (FP_Gun != nullptr) FP_Gun->SetVisibility(false);
}

void AThirdPersonCharacter::SetRotOnServer_Implementation(FRotator rot)
{
	SetRotOnMulti(rot);
}

void AThirdPersonCharacter::SetRotOnMulti_Implementation(FRotator rot)
{
	GetActorRotation() = rot;
}


void AThirdPersonCharacter::ThrowGunOnServer_Implementation()
{
	ThrowGunOnMulti();
}

void AThirdPersonCharacter::InputFireOnServer_Implementation()
{
	InputFireOnMulti();
}

void AThirdPersonCharacter::ThrowGunOnMulti_Implementation()
{
	if (EquipedGuns[curGun].IsValid()) {
		EquipedGuns[curGun]->GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		EquipedGuns[curGun]->Set_Equiped(false);
		EquipedGuns[curGun]->SetMyOwner(nullptr);

		EquipedGuns[curGun] = nullptr;
		OnRep_UpdatedEquipGun();
	}
}

void AThirdPersonCharacter::InputFireOnMulti_Implementation()
{
	if (EquipedGuns[curGun].IsValid() && EquipedGuns[curGun]->bEquiped && EquipedGuns[curGun]->Get_Owner() == this) {

		if (EquipedGuns[curGun]->Get_C_Armo() > 0) {
			if (GetMovementComponent()->IsFalling()) {
				EquipedGuns[curGun]->bSpread = true;
			}
			else {
				EquipedGuns[curGun]->bSpread = false;
			}

			EquipedGuns[curGun]->Fire(AimShotPoint);

			if (EquipedGuns[curGun].IsValid())
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT(" cur Gun : %d "), curGun));

			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("fire_ Equipe")));
		}
		else {

			EquipedGuns[curGun]->EndFire();
			Reloading();
		}
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("fire_NO Equipe")));
	}
}

void AThirdPersonCharacter::OnRep_UpdatedEquipGun()
{
}

void AThirdPersonCharacter::OnRep_UpdatedSearchGun()
{
}

void AThirdPersonCharacter::SetPitchYawOnServer_Implementation(float DeltaTime)
{
	SetPitchYaw(DeltaTime);
}

void AThirdPersonCharacter::InputReload()
{
	ReloadingOnServer();
}

void AThirdPersonCharacter::Reloading()
{
	if (Zoom) {
		ChangeCamera();
	}
	if (!anim->Montage_IsPlaying(Anim_Reload))
		PlayAnimMontageOnServer(Anim_Reload);


	anim->OnDelicate_GunReload_Start.Clear();
	anim->OnDelicate_GunReload_End.Clear();

	if (EquipedGuns[curGun].IsValid()) {
		anim->OnDelicate_GunReload_Start.AddLambda([this]() -> void {
			ArmoClear();
			});
		anim->OnDelicate_GunReload_End.AddLambda([this]() -> void {
			ArmoFill();
			});
	}
}

void AThirdPersonCharacter::ReloadingOnServer_Implementation()
{
	ReloadingOnMulti();
}

void AThirdPersonCharacter::ReloadingOnMulti_Implementation()
{
	Reloading();
}

void AThirdPersonCharacter::ArmoClear()
{
	if (EquipedGuns[curGun].IsValid()) {
		EquipedGuns[curGun]->Set_C_Armo(0);

	}

}

void AThirdPersonCharacter::ArmoFill()
{
	if (EquipedGuns[curGun].IsValid()) {
		EquipedGuns[curGun]->Set_C_Armo(EquipedGuns[curGun]->Get_M_Armo());

	}

}

void AThirdPersonCharacter::OnRep_UpdatedAimDirection()
{
}

void AThirdPersonCharacter::OnRep_UpdatedStatSetting()
{
}

float AThirdPersonCharacter::Get_HpRatio()
{
	return 0.f;
}

ETeamEnum AThirdPersonCharacter::GetTeam()
{
	return MyTeam;
}

void AThirdPersonCharacter::SetTeam(ETeamEnum newTeam)
{
	ApplyTeam(newTeam);
	SetTeamOnServer(newTeam);
}

void AThirdPersonCharacter::ApplyTeam(ETeamEnum newTeam)
{
	MyTeam = newTeam;
	if (MyTeam == ETeamEnum::RedTeam) {
		GetMesh()->SetMaterial(0, Mats[0]);
	}
	else if (MyTeam == ETeamEnum::BlueTeam) {
		GetMesh()->SetMaterial(0, Mats[1]);
	}
}

void AThirdPersonCharacter::SetTeamOnServer_Implementation(ETeamEnum newTeam)
{
	SetTeamOnMulti(newTeam);
}

void AThirdPersonCharacter::SetTeamOnMulti_Implementation(ETeamEnum newTeam)
{
	ApplyTeam(newTeam);
}

void AThirdPersonCharacter::InputSkill0()
{
	Character_Skill_Component->InputSkill0();
}

void AThirdPersonCharacter::InputSkill1()
{
	Character_Skill_Component->InputSkill1();
}

void AThirdPersonCharacter::InputSelectGun_0()
{
	if (Zoom) return;

	if (EquipedGuns[0].IsValid() && curGun == 1) {
		PlayAnimMontageOnServer(Anim_ChangeGun);
		if (anim) {
			anim->OnDelicate_ChangeGun.Clear();
			anim->OnDelicate_ChangeGun.AddUObject(this, &AThirdPersonCharacter::ChangeGun_0);

		}
	}
}

void AThirdPersonCharacter::ChangeGun_0()
{
	if (EquipedGuns[0].IsValid() && curGun == 1) {
		curGun = 0;
		EquipedGuns[0]->GetSK_Mesh()->SetVisibility(true);

		if (EquipedGuns[1].IsValid()) {
			EquipedGuns[1]->GetSK_Mesh()->SetVisibility(false);
		}

		plController->GetCrosshairUI()->SelectFirstGun();
	}
	SelectGun_0();
	SelectGun_0OnServer();
	SelectGun_0OnMulti();
}

void AThirdPersonCharacter::SelectGun_0()
{
	curGun = 0;
	if (EquipedGuns[0].IsValid()) {
		EquipedGuns[0]->AttachToComponent(ShotPoint, FAttachmentTransformRules::SnapToTargetIncludingScale, NAME_None);
	}
	if (EquipedGuns[1].IsValid()) {
		EquipedGuns[1]->GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void AThirdPersonCharacter::SelectGun_0OnMulti_Implementation()
{
	if (!HasAuthority()) {
		SelectGun_0();
	}
}

void AThirdPersonCharacter::SelectGun_0OnServer_Implementation()
{
	if (HasAuthority()) {
		SelectGun_0OnMulti();
		SelectGun_0();
	}
}

void AThirdPersonCharacter::InputSelectGun_1()
{
	if (Zoom) return;
	if (EquipedGuns[1].IsValid() && curGun == 0) {
		PlayAnimMontageOnServer(Anim_ChangeGun);
		if (anim) {
			anim->OnDelicate_ChangeGun.Clear();
			anim->OnDelicate_ChangeGun.AddUObject(this, &AThirdPersonCharacter::ChangeGun_1);
		}
	}

}

void AThirdPersonCharacter::ChangeGun_1()
{
	if (EquipedGuns[1].IsValid() && curGun == 0) {
		curGun = 1;
		EquipedGuns[1]->GetSK_Mesh()->SetVisibility(true);

		if (EquipedGuns[0].IsValid()) {
			EquipedGuns[0]->GetSK_Mesh()->SetVisibility(false);
		}

		plController->GetCrosshairUI()->SelectSecondGun();
	}

	SelectGun_1();
	SelectGun_1OnServer();
	SelectGun_1OnMulti();
}

void AThirdPersonCharacter::SelectGun_1()
{
	curGun = 1;
	if (EquipedGuns[1].IsValid()) {
		EquipedGuns[1]->AttachToComponent(ShotPoint, FAttachmentTransformRules::SnapToTargetIncludingScale, NAME_None);
	}
	if (EquipedGuns[0].IsValid()) {
		EquipedGuns[0]->GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}


void AThirdPersonCharacter::SelectGun_1OnMulti_Implementation()
{
	if (!HasAuthority()) {
		SelectGun_1();
	}
}

void AThirdPersonCharacter::SelectGun_1OnServer_Implementation()
{
	if (HasAuthority()) {
		SelectGun_1OnMulti();
		SelectGun_1();
	}
}

void AThirdPersonCharacter::SetFireSyncTransformOnServer_Implementation(FTransform transform)
{
	SyncTransform = transform;
	if (IsLocallyControlled()) return;

	ShotPoint->SetWorldTransform(SyncTransform);
}

void AThirdPersonCharacter::SetZoom(bool _zoom)
{
	Zoom = _zoom;
	if (Zoom) {
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = 700.f;
	}
}

void AThirdPersonCharacter::SetScopeTarget()
{
	if (IsLocallyControlled()) {
		ScopeCamera->TextureTarget = ScopeRenderImage;
	}
}

void AThirdPersonCharacter::SetSetting()
{
	//OnRep_UpdatedStatSetting();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, GetName() + FString::Printf(TEXT(" : HP : %f"), CurrentHP));
}

void AThirdPersonCharacter::BindFloatingHP()
{
	/*floatingHP = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP_Bar"));
	floatingHP->SetupAttachment(GetMesh());

	floatingHP->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	floatingHP->SetWidgetSpace(EWidgetSpace::Screen);
	FString uipath = TEXT("WidgetBlueprint'/Game/UI/FloationHP.FloationHP_C'");
	static ConstructorHelpers::FClassFinder<UUserWidget> widget(*uipath);
	if (widget.Succeeded()) {
		floatingHP->SetWidgetClass(widget.Class);
		floatingHP->SetDrawSize(FVector2D(60.0f, 15.0f));
	}*/
}

void AThirdPersonCharacter::InputZoom()
{
	if (anim->Montage_IsPlaying(Anim_Reload)) return;

	if (bzooming) {
		bzooming = false;
		FollowCamera->SetRelativeLocation(ThirdPersonCameraLoc);
	}
	else {
		ThirdPersonCameraLoc = FollowCamera->GetRelativeLocation();
		bzooming = true;
		PlayAnimMontageOnServer(Anim_Zoom);
	}
}

void AThirdPersonCharacter::Zooming(float DeltaTime)
{
	if (bzooming) {
		ZoomTime += DeltaTime;
		FVector cameraLoc = FMath::Lerp(ThirdPersonCameraLoc, FP_Camera->GetRelativeLocation(), ZoomTime / 0.2f);
		FollowCamera->SetRelativeLocation(cameraLoc);

		if (ZoomTime >= 0.25f) {
			ZoomTime = 0.0f;
			bzooming = false;
			ChangeCamera();
			FollowCamera->SetRelativeLocation(ThirdPersonCameraLoc);
		}
	}


}

void AThirdPersonCharacter::ZoomOff_Implementation()
{
	if (EquipedGuns[curGun].IsValid()) {

		if (EquipedGuns[curGun]->Get_C_Armo() <= 0 && Zoom) {
			ChangeCamera();
		}
		FString armo;
		armo = FString::FromInt(EquipedGuns[curGun]->Get_C_Armo());
		plController->GetCrosshairUI()->UpdateArmo(armo, curGun);

	}
}

void AThirdPersonCharacter::SetZoomOnServer_Implementation(bool _zoom)
{
	SetZoom(_zoom);
}

void AThirdPersonCharacter::SearchGunOnServer_Implementation(class AMyGunActor* hitActor)
{
	SearchGunOnMulti(hitActor);
}

void AThirdPersonCharacter::SearchGunOnMulti_Implementation(class AMyGunActor* hitActor)
{
	SearchGun = hitActor;
	OnRep_UpdatedSearchGun();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Server bEquiped : %d"), weapon->Get_Equiped()));
}

void AThirdPersonCharacter::Equip2()
{
	if (SearchGun) {
		if (!EquipedGuns[0].IsValid()) {
			Equip_FirstGun();
			Equip_FirstGunOnServer();
			Equip_FirstGunOnMulti();
			plController->GetCrosshairUI()->GetFirstGun("AR");
			ChangeGun_0();

			FString armo;
			armo = FString::FromInt(EquipedGuns[curGun]->Get_C_Armo());
			plController->GetCrosshairUI()->UpdateArmo(armo, curGun);
		}
		else if (!EquipedGuns[1].IsValid()) {
			Equip_SecondGun();
			Equip_SecondGunOnServer();
			Equip_SecondOnMulti();
			plController->GetCrosshairUI()->GetSecondGun("AR");
			ChangeGun_1();

			FString armo;
			armo = FString::FromInt(EquipedGuns[curGun]->Get_C_Armo());
			plController->GetCrosshairUI()->UpdateArmo(armo, curGun);
		}

		//FP_Camera->SetRelativeLocation(FVector(0, 0, 0));
		//FP_Camera->AttachToComponent(EquipedGun->GetSKMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "AimSight");
	}
}

void AThirdPersonCharacter::Equip_FirstGun()
{
	if (SearchGun) {
		EquipedGuns[0] = SearchGun;
		OnRep_UpdatedEquipGun();

		EquipedGuns[0]->AttachToComponent(ShotPoint, FAttachmentTransformRules::SnapToTargetIncludingScale, NAME_None);

		EquipedGuns[0]->Set_Equiped(true);

		EquipedGuns[0]->SetMyOwner(this);
	}
}

void AThirdPersonCharacter::Equip_FirstGunOnServer_Implementation()
{
	if (SearchGun) {
		if (HasAuthority()) {
			Equip_FirstGunOnMulti();
			Equip_FirstGun();
		}
	}
}

void AThirdPersonCharacter::Equip_FirstGunOnMulti_Implementation()
{
	if (!HasAuthority()) {
		Equip_FirstGun();
	}
}

void AThirdPersonCharacter::Equip_SecondGun()
{
	if (SearchGun) {
		EquipedGuns[1] = SearchGun;
		OnRep_UpdatedEquipGun();

		EquipedGuns[1]->AttachToComponent(ShotPoint, FAttachmentTransformRules::SnapToTargetIncludingScale, NAME_None);

		EquipedGuns[1]->Set_Equiped(true);

		EquipedGuns[1]->SetMyOwner(this);

	}

}

void AThirdPersonCharacter::ThrowGun()
{
	if (EquipedGuns[curGun].IsValid()) {
		ThrowGunOnServer();
		plController->GetCrosshairUI()->ThrowGun(curGun);
	}
}

void AThirdPersonCharacter::Equip_SecondGunOnServer_Implementation()
{
	if (HasAuthority()) {
		Equip_SecondOnMulti();
		Equip_SecondGun();
	}
}

void AThirdPersonCharacter::Equip_SecondOnMulti_Implementation()
{
	if (!HasAuthority()) {
		Equip_SecondGun();
	}
}

void AThirdPersonCharacter::Equip2OnServer_Implementation()
{
	if (HasAuthority()) {
		Equip2OnMulti();
		Equip2();
	}
}

void AThirdPersonCharacter::Equip2OnMulti_Implementation()
{
	if (!HasAuthority()) {
		Equip2();
	}

}
