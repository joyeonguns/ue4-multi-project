// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGunActor.h"
#include "Net/UnrealNetwork.h"
#include <Kismet/GameplayStatics.h>
#include "MyMatineeCameraShake.h"
#include "ThirdPersonCharacter.h"
#include "Math/UnrealMathUtility.h"
#include "MyGameInstance.h"

// Sets default values
AMyGunActor::AMyGunActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("GunBoxComponent"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("Firepoint"));

	RootComponent = ItemBoxComponent;
	Mesh->AttachToComponent(ItemBoxComponent, FAttachmentTransformRules::KeepRelativeTransform); //AttachTo(ItemBoxComponent);
	FirePoint->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform); //AttachTo(ItemBoxComponent);

	//FirePoint->SetupAttachment(Mesh, TEXT("Muzzle"));
	//AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Muzzle"));

	Damage = 15.0f;

	SetReplicates(true);
	SetReplicateMovement(true);

	MyTimeLineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("MyTimelineComp"));

}

// Called when the game starts or when spawned
void AMyGunActor::BeginPlay()
{
	Super::BeginPlay();

	M_Armo = MyGunData.Armo;

	Set_C_Armo(M_Armo);

	GunOwner = nullptr;

	OnRep_UpdateEquip();
	OnRep_UpdateEquip();

	if (YawCurve != nullptr && PitchCurve != nullptr && MyTimeLineComp != nullptr)
	{
		// Callback 함수에 사용할 함수를 바인드합니다.// 지정한 Curve에 사용할 Callback 함수들입니다.
		FOnTimelineFloat YawCurveCallback;
		FOnTimelineFloat PitchCurveCallback;

		// Timeline이 끝났을 때 실행할 Callback 함수들입니다.
		FOnTimelineEvent LerpTimelineFinishedCallback;
		// 바인드 하는 함수에는 UFUNCTION 매크로가 적용되어야합니다.
		YawCurveCallback.BindUFunction(this, FName("RecoilInputYaw"));
		PitchCurveCallback.BindUFunction(this, FName("RecoilInputPitch"));
		LerpTimelineFinishedCallback.BindUFunction(this, FName("RecoilFinish"));

		// Timeline에 Curve와 Curve를 사용할 Callback 함수를 추가합니다.
		MyTimeLineComp->AddInterpFloat(YawCurve, YawCurveCallback);
		MyTimeLineComp->AddInterpFloat(PitchCurve, PitchCurveCallback);

		// Timeline을 끝낼때 호출할 Cabllback 함수를 추가합니다.
		MyTimeLineComp->SetTimelineFinishedFunc(LerpTimelineFinishedCallback);

		// Timeline의 길이를 설정합니다.
		MyTimeLineComp->SetTimelineLength(0.0f);
		MyTimeLineComp->SetPlayRate(1.0f);
		MyTimeLineComp->SetLooping(false);

		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT(" SetTimeLine : %f "), MyTimeLineComp->GetTimelineLength()));
		RecoilTimeLine.SetTimelineLength(10.0f);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT(" SetTimeLine : %f "), MyTimeLineComp->GetTimelineLength()));

		//MyTimeLineComp->PlayFromStart();
	}
}

// Called every frame
void AMyGunActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (fireCooldown > 0) {
		fireCooldown -= DeltaTime;
	}

	if (MyTimeLineComp != nullptr) {
		//MyTimeLineComp->
	}


	if (HasAuthority()) {
		SyncFirePointTransform(FirePoint->GetComponentTransform());
		
	}
	else {
		SyncFirePointTransform(FirePoint->GetComponentTransform());
	}


}

void AMyGunActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyGunActor, C_Armo);
	DOREPLIFETIME(AMyGunActor, bEquiped);
	DOREPLIFETIME(AMyGunActor, GunOwner);
	DOREPLIFETIME(AMyGunActor, SpawnLoc);
	DOREPLIFETIME(AMyGunActor, YawCurve);
	DOREPLIFETIME(AMyGunActor, PitchCurve);
	DOREPLIFETIME(AMyGunActor, RecoilTimeLine);
	DOREPLIFETIME(AMyGunActor, RecoilRecovery);
	DOREPLIFETIME(AMyGunActor, OriginRotation);
	DOREPLIFETIME(AMyGunActor, PostRecoilRotation);
	DOREPLIFETIME(AMyGunActor, fireCooldown);
	
}

USkeletalMeshComponent* AMyGunActor::GetSK_Mesh()
{
	return Mesh;
}

void AMyGunActor::SetOwner(class AThirdPersonCharacter* _Owner)
{
	GunOwner = _Owner;
}

AThirdPersonCharacter* AMyGunActor::Get_Owner()
{
	return GunOwner;
}

float AMyGunActor::Get_Damage()
{
	return Damage;
}

void AMyGunActor::Set_Equiped(bool _equip)
{
	bEquiped = _equip;
	OnRep_UpdateEquip();
}

bool AMyGunActor::Get_Equiped()
{
	return bEquiped;
}

int32 AMyGunActor::Get_C_Armo()
{
	return C_Armo;
}

void AMyGunActor::Set_C_Armo(int32 armo)
{
	Set_curArmoOnServer(armo);
	Set_curArmoOnMulti(armo);
	OnRep_UpdatedC_Armo();
	
}

void AMyGunActor::Set_curArmo(int32 armo)
{
	C_Armo = armo;
	OnRep_UpdatedC_Armo();
	
}

void AMyGunActor::Set_curArmoOnMulti_Implementation(int32 armo)
{
	Set_curArmo(armo);	
	
}

void AMyGunActor::Set_curArmoOnServer_Implementation(int32 armo)
{
	//Set_curArmo(armo);
	Set_curArmoOnMulti(armo);
	if (!HasAuthority() && GunOwner) {
		GunOwner->UpdateArmoUI(C_Armo, M_Armo);
	}
}

int32 AMyGunActor::Get_M_Armo()
{
	return M_Armo;
}

void AMyGunActor::Set_M_Armo(int32 armo)
{
	M_Armo = armo;
}

void AMyGunActor::FireOnServer_Implementation()
{
	if (!HasAuthority() || !GunOwner->HasAuthority()) return;

	//SpawnLoc = GetActorLocation() + GetActorForwardVector() * 100.f; //
	SpawnLoc = FirePoint->GetComponentLocation();


	if (GunOwner == nullptr) return;

	
	
	FVector AddVec = FVector(GetActorForwardVector().Y, -1 * GetActorForwardVector().X, GetActorForwardVector().Z);
	FVector spwLoc = SpawnLoc;
	FRotator spwRot = GunOwner->GetActorRotation();

	FActorSpawnParameters spwParam;

	FVector dir;
	// 줌
	if (GunOwner->bZoom) {
		dir = GunOwner->AimShotPoint;
	}
	else {
		dir = GunOwner->HipShotPoint;
	}

	dir = (dir - spwLoc).GetSafeNormal();

	// 점프시
	if (bSpread) {
		float spread_X;
		float spread_Y;
		float spread_Z;
		float rad = 0;
		float digree;
		FVector spreadVec;
		spread_X = FMath::RandRange(-1, 1);
		spread_Y = FMath::RandRange(-1, 1);
		spread_Z = FMath::RandRange(-1, 1);

		spreadVec = FVector(spread_X, spread_Y, spread_Z).GetSafeNormal();

		spreadVec = dir + spreadVec * 0.05;

		rad = FMath::Acos(FVector::DotProduct(dir, spreadVec));
		digree = FMath::RadiansToDegrees(rad);

		dir = spreadVec;
	}

	if (HasAuthority() && GetWorld()) {
		auto ProjectileInstance = GetWorld()->SpawnActor<AActor_Projectile>(Projectiles, spwLoc, dir.Rotation(), spwParam);

		if (ProjectileInstance) {
			ProjectileInstance->SetActorScale3D(FVector(0.1f, 0.1f, 0.1f));
			ProjectileInstance->SetSetting(MyGunData.Damage, dir, MyGunData.Speed, EProjectileEnum::LightShot, GunOwner->GetTeam(), GunOwner, MyGunData.AimPunch);

			ProjectileInstance->Fire(dir);
		}

		/*int32 newArmo = C_Armo - 1;
		Set_C_Armo(newArmo);*/
	}
}

void AMyGunActor::ApplyRecoilOnMulti_Implementation(FVector dir)
{
	if (fireCooldown <= 0 && C_Armo > 0) {
		float spread_y = FMath::RandRange(0.f, MyGunData.Spread_Y * -1);

		float spread_x = FMath::RandRange(MyGunData.Spread_Y * -1, MyGunData.Spread_Y);

		if (GunOwner != nullptr) {
			GunOwner->AddControllerPitchInput(spread_y);
			GunOwner->AddControllerYawInput(spread_x);

			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT(" Spread : %f, %f "), spread_x, spread_y));
		}
	}
}

void AMyGunActor::ChargeArmoOnServer_Implementation(int32 charge)
{
	ChargeArmoOnMulti(charge);
}

void AMyGunActor::ChargeArmoOnMulti_Implementation(int32 charge)
{
	C_Armo = charge;
	OnRep_UpdatedC_Armo();
}

void AMyGunActor::OnRep_UpdatedC_Armo()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT(" Armo : %d "), C_Armo));
}

void AMyGunActor::OnRep_UpdateEquip()
{

}

void AMyGunActor::OnRep_UpdatedMyOwner()
{

}

void AMyGunActor::SetMyOwner(AThirdPersonCharacter* _owner)
{
	GunOwner = _owner;
	OnRep_UpdatedMyOwner();
}

void AMyGunActor::Fire(FVector dir)
{
	if (MyGunData.GunType == "Single") {
		SingleFire();
	}
	else if (MyGunData.GunType == "Auto") {
		AutoFire();
	}
}

void AMyGunActor::AutoFire()
{
	SingleFire();
	GetWorldTimerManager().SetTimer(TH_AutoFire, this, &AMyGunActor::SingleFire, MyGunData.ShotTime / 2, true);

	// Timeline을 실행합니다.
	if (HasAuthority()) {

		PlayRecoilTimeLineOnMulti();
		//PlayRecoilTimeLineOnServer();;
		if (GunOwner && GunOwner->HasAuthority()) {
			//GunOwner->InputFireOnMulti();
		}
	}
}

void AMyGunActor::SingleFire()
{
	if (fireCooldown <= 0 && C_Armo > 0) {
		
	}

	if (HasAuthority()) {
		if (fireCooldown <= 0 && C_Armo > 0) {

			fireCooldown = MyGunData.ShotTime;
			
			Set_C_Armo(C_Armo -1);

			FireOnServer();

			UMyMatineeCameraShake* CameraShake = NewObject<UMyMatineeCameraShake>();

			APlayerController* OwnerController = Cast<APlayerController>(GunOwner->GetController());
			if (OwnerController) {
				OwnerController->ClientStartCameraShake(CameraShake->StaticClass(), 1.0f);
			}

			
		}
		else if (fireCooldown <= 0 && C_Armo <= 0) {
			if (GunOwner == nullptr) return;
			EndFire();
			GunOwner->ReloadingOnServer();
			//MyOwner->ZoomOff();
		}
	}

	if (GunOwner == nullptr) return;

	
}

void AMyGunActor::EndFire()
{
	MyTimeLineComp->Stop();
	GetWorldTimerManager().ClearTimer(TH_AutoFire);

	MyTimeLineComp->SetPlayRate(5.0f);
	RecoilRecovery = true;
	PostRecoilRotation = GunOwner->GetControlRotation();
	MyTimeLineComp->Reverse();
}

void AMyGunActor::RecoilInputYaw(float x)
{
	RecoilInputYawOnServer(x);
}

void AMyGunActor::RecoilInputYawOnMulti_Implementation(float x)
{
	if (RecoilRecovery) {
		if (GunOwner != nullptr) {
			FRotator rot = OriginRotation - PostRecoilRotation;
			float ClampYaw = FMath::Clamp(rot.Pitch, 0.0f, 1.0f);
			GunOwner->AddControllerYawInput(x * -1 * ClampYaw);
			
		}
	}
	else {
		if (fireCooldown <= 0 && C_Armo > 0) {
			if (GunOwner != nullptr) {
				GunOwner->AddControllerYawInput(x);
			}
		}
	}

}

void AMyGunActor::RecoilInputYawOnServer_Implementation(float x)
{
	RecoilInputYawOnMulti(x);
}

void AMyGunActor::RecoilInputPitch(float x)
{
	RecoilInputPitchOnServer(x);
}

void AMyGunActor::RecoilInputPitchOnMulti_Implementation(float x)
{
	if (RecoilRecovery) {
		if (GunOwner != nullptr) {
			FRotator rot = OriginRotation - PostRecoilRotation;
			float ClampPitch = FMath::Clamp(rot.Pitch, 0.0f, 1.0f);

			GunOwner->AddControllerPitchInput(x * -1 * ClampPitch);
		}
	}
	else {
		if (fireCooldown <= 0 && C_Armo > 0) {
			if (GunOwner != nullptr) {
				GunOwner->AddControllerPitchInput(x);

				if (!HasAuthority()) {
					FRotator rot = OriginRotation - PostRecoilRotation;
					float ClampPitch = FMath::Clamp(rot.Pitch, 0.0f, 1.0f);

					//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT(" pitch : %f "), ClampPitch));
				}

			}
		}
	}

}

void AMyGunActor::RecoilInputPitchOnServer_Implementation(float x)
{
	RecoilInputPitchOnMulti(x);
}

void AMyGunActor::RecoilFinish()
{

}

void AMyGunActor::RecoilFinishOnMulti()
{
}

void AMyGunActor::Equip(class AThirdPersonCharacter* _Owner)
{
	EquipOnServer(_Owner);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT(" equip ")));
}

void AMyGunActor::EquipOnServer_Implementation(class AThirdPersonCharacter* _Owner)
{
	EquipOnMulti(_Owner);
}

void AMyGunActor::EquipOnMulti_Implementation(class AThirdPersonCharacter* _Owner)
{
	OnRep_UpdateEquip();
	OnRep_UpdatedMyOwner();

	/*if (MyOwner == _Owner) {
		GetRootComponent()->AttachToComponent(MyOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Gun_R"));
	}*/

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("  Equiped  ")));
}

void AMyGunActor::SetSpawnLoc(FVector spwloc)
{
	SpawnLoc = spwloc;
}

USkeletalMeshComponent* AMyGunActor::GetSKMesh()
{
	return Mesh;
}

void AMyGunActor::PlayRecoilTimeLine()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("  PlayRecoilTimeLine  ")));
	MyTimeLineComp->SetPlayRate(1.0f);
	RecoilRecovery = false;
	OriginRotation = GunOwner->GetControlRotation();
	MyTimeLineComp->PlayFromStart();
}

float AMyGunActor::GetFireCoolDown()
{
	return fireCooldown;
}

void AMyGunActor::ChangeArmoOnClient_Implementation(int32 newArmo)
{
	C_Armo = newArmo;
	OnRep_UpdatedC_Armo();
}

void AMyGunActor::ChangeArmoOnServer_Implementation(int32 newArmo)
{
	C_Armo = newArmo;
	ChangeArmoOnClient(newArmo);
}

void AMyGunActor::SyncFirePointTransform_Implementation(FTransform transform)
{
	FirePointLoc = transform;
	if (GunOwner != nullptr && GunOwner->IsLocallyControlled() ) return;

	FirePoint->SetWorldTransform(FirePointLoc);
}

void AMyGunActor::PlayRecoilTimeLineOnServer_Implementation()
{
	PlayRecoilTimeLineOnMulti();
}

void AMyGunActor::PlayRecoilTimeLineOnMulti_Implementation()
{
	PlayRecoilTimeLine();
}

