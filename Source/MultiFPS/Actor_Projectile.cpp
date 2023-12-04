// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_Projectile.h"
#include "Net/UnrealNetwork.h"
#include "Engine/DecalActor.h"
#include "ThirdPersonCharacter.h"
#include "Components/DecalComponent.h"
#include "Actor_DamageText.h"
#include "Character_State_Component.h"
#include <Kismet/GameplayStatics.h>
#include "MyMatineeCameraShake.h"
#include <PhysicalMaterials/PhysicalMaterial.h>
#include "MyGameInstance.h"

// Sets default values
AActor_Projectile::AActor_Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(RootComponent);
	ProjectileMovement->InitialSpeed = 100.f; // 초기 속도 설정
	ProjectileMovement->MaxSpeed = 100.f;

	RootComponent = SphereComponent;
	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform); //AttachTo();


	//ProjectileMovement->InitialSpeed = 3000.f;

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AActor_Projectile::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) {
		OnRep_UpdateSetting();
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("%f , %f , %f, "), MoveDirection.X, MoveDirection.Y, MoveDirection.Z));

	}

	//SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AActor_Projectile::OnOverlapBegin);

	//SphereComponent->OnComponentHit.AddDynamic(this, &AActor_Projectile::OnHit);

	if (FireSound) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("FireSound Play")));
		float soundValue = 1.0f;
		UMyGameInstance* myGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (myGameInstance) soundValue = myGameInstance->AudioSound;
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation(), GetActorRotation(), soundValue);
	}

}

void AActor_Projectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AActor_Projectile, Damage);
	DOREPLIFETIME(AActor_Projectile, Speed);
	DOREPLIFETIME(AActor_Projectile, ProjectileType);
	DOREPLIFETIME(AActor_Projectile, MoveDirection);
	DOREPLIFETIME(AActor_Projectile, Team);
	DOREPLIFETIME(AActor_Projectile, BulletOwner);
	DOREPLIFETIME(AActor_Projectile, AimPunch);
}

// Called every frame
void AActor_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bendMove) {
		SetActorLocationOnServer(MoveDirection);
		Fire(MoveDirection);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT(" vec ")));

		bendMove = true;
	}

	if (HasAuthority()) {
		if (LifeTime > 0.0f) {
			LifeTime -= DeltaTime;
		}
		else {
			DestroyObjectOnServer();
		}
	}

	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 1000; // DeltaTime에 투사체 속도를 곱해 이동 거리 계산

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // 자신을 무시

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldDynamic, CollisionParams))
	{
		if (HitResult.GetComponent()->ComponentHasTag("Capsule")) return;
		
		// 충돌 감지, OnHit 함수 호출
		OnHit(SphereComponent, HitResult.GetActor(), HitResult.GetComponent(), HitResult.ImpactPoint, HitResult);
	}
}

void AActor_Projectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this) {
		if (OtherActor->ActorHasTag("Player")) {
			HitOnServer(OtherActor);
		}
		else if (!OtherActor->ActorHasTag("Gun")) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Overlap : ")) + OtherActor->GetName());
			DestroyObjectOnServer();
		}

		DestroyObjectOnServer();
	}

}

void AActor_Projectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{	
	bool bCrit = false;
	int Dmg = Damage;

	if (HasAuthority()) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT(" OnHit-HasAuthority ")));
		if (OtherActor->ActorHasTag("Gun")) return;

		
		

		auto Target = Cast<AThirdPersonCharacter>(OtherActor);
		if (Target) {
			/*FString b_Team, T_Team;
			const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ETeamEnum"), true);
			if (!enumPtr)
			{
				b_Team = "Not";
				T_Team = "Not";
			}
			else {
				b_Team = enumPtr->GetNameStringByIndex((int32)Team);
				T_Team = enumPtr->GetNameStringByIndex((int32)Target->GetTeam());
			}

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, b_Team + FString::Printf(TEXT(" -> ")) + T_Team);*/

			// 플레이어 타격
			if (Target->GetTeam() != Team) {
				
				float distance = FVector::Distance(Target->GetMesh()->GetSocketLocation("headsocket"), Hit.ImpactPoint);
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT(" distance  %f"), distance));
				
				// 크리티컬
				if (distance < 30.f) {
					bCrit = true;
					Dmg = Damage * 2;
				}

				// Floating 데미지 생성 

				// 에임펀치
				Target->CameraShakeComp->ShakeCamera(1.0f, 15.0f * (bCrit + 1));

				if (Target->Character_State_Component->GetShield() > 0) {
					// Shield
					if (BulletOwner != nullptr) {
						BulletOwner->Comp_FloatingDamage->SpawnDamageTextActorOnClient(Dmg, Hit.ImpactPoint, 1, bCrit);
					}
				}
				else {
					// Hp
					if (BulletOwner != nullptr) {
						BulletOwner->Comp_FloatingDamage->SpawnDamageTextActorOnClient(Dmg, Hit.ImpactPoint, 2, bCrit);
						
						
					}
				}

				UGameplayStatics::ApplyDamage(OtherActor, Dmg, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, UDamageType::StaticClass());
			}
		}
		else {
			// 더미 타겟
			if (OtherActor->ActorHasTag("TrainingBot")) {
				if (BulletOwner != nullptr) {
					if (OtherComponent->ComponentHasTag("Head")) {
						bCrit = true;
						Dmg = Damage * 2;
					}
					BulletOwner->Comp_FloatingDamage->SpawnDamageTextActorOnClient(Dmg, Hit.ImpactPoint, 0, bCrit);
				}
			}
		}		
	}
	else {

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT(" OnHit-HasAuthority X  ")));

	}

	SpawnBulletHole(OtherComponent, Hit.ImpactPoint, Hit.ImpactNormal);
}

void AActor_Projectile::Move(FVector dir)
{
	SetActorLocationOnServer(dir);
}

void AActor_Projectile::OnRep_UpdateSetting()
{

}

void AActor_Projectile::HitOnServer_Implementation(AActor* HitActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Hit Projectile : ") + HitActor->GetName());

	FDamageEvent DamageEvent;
	HitActor->TakeDamage(20, DamageEvent, nullptr, this);
	if (ProjectileType != EProjectileEnum::SniperShot) {
		DestroyObjectOnServer();
	}


	HittOnClient(HitActor);
}

void AActor_Projectile::HittOnClient_Implementation(AActor* HitActor)
{

}

void AActor_Projectile::SetActorLocationOnServer_Implementation(FVector newLocation)
{
	SetActorLocationOnClient(newLocation);
}

void AActor_Projectile::SetActorLocationOnClient_Implementation(FVector newLocation)
{
	//ProjectileMovement->Velocity = newLocation * 50,000,000.0f; //Speed;
	ProjectileMovement->Velocity = newLocation * Speed;
}

void AActor_Projectile::DestroyObjectOnServer_Implementation()
{
	DestroyObjectOnClient();
}

void AActor_Projectile::DestroyObjectOnClient_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Destroy Object"));
	Destroy();
}

void AActor_Projectile::SpawnBulletHole_Implementation(UPrimitiveComponent* OtherComponent, FVector ImpactPoint, FVector ImpactNormal)
{
	UGameplayStatics::SpawnDecalAttached(
		Decal_Bullethole,
		FVector(15.0f, 15.0f, 15.0f),
		OtherComponent,
		NAME_None,
		ImpactPoint,
		ImpactNormal.Rotation(),
		EAttachLocation::KeepWorldPosition,
		10.0f);


	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("SpawnBulletHole"));

	Destroy();
}

void AActor_Projectile::Setloc(FVector loc)
{
	SetActorLocation(loc);

}

void AActor_Projectile::Fire(FVector dir)
{
	FireDirection(dir);
	FireDirectionOnServer(dir);

}

void AActor_Projectile::FireDirection(FVector dir)
{	
	//ProjectileMovement->Velocity = dir * ProjectileMovement->InitialSpeed;

	//SphereComponent->AddImpulse(dir * 500.f, NAME_None, true);
	
	//SphereComponent->AddImpulse(dir * Speed, NAME_None, true);
}

void AActor_Projectile::SetSetting(float _damage, FVector _dir, float _speed, EProjectileEnum _type, ETeamEnum _team, AThirdPersonCharacter* _BulletOwner, float _AimPunch)
{
	Damage = _damage;
	MoveDirection = _dir;
	Speed = _speed;
	ProjectileType = _type;
	Team = _team;
	BulletOwner = _BulletOwner;
	AimPunch = _AimPunch;

	OnRep_UpdateSetting();
}

void AActor_Projectile::FireDirectionOnServer_Implementation(FVector dir)
{
	FireDirection(dir);
}

void AActor_Projectile::FireDirectionOnMulti_Implementation(FVector dir)
{
	FireDirection(dir);
}

void AActor_Projectile::SpawnTextureOnMulti_Implementation(int32 idx)
{
	if (NiagaraComp) {
		//UNiagaraFunctionLibrary::SetTextureObject(NiagaraComp, FString("user.Digit1"), DamageFonts[idx]);
	}
}

void AActor_Projectile::ShootOnClient_Implementation(FVector dir)
{
	MoveDirection = (MoveDirection - GetActorLocation()).GetSafeNormal();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("%f , %f , %f, "), MoveDirection.X, MoveDirection.Y, MoveDirection.Z));

	ProjectileMovement->AddForce(FVector::ZeroVector);
}

void AActor_Projectile::ShootOnServer_Implementation(FVector dir)
{
	ShootOnClient(dir);
}
