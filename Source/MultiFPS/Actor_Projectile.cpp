// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_Projectile.h"
#include "Net/UnrealNetwork.h"
#include "Engine/DecalActor.h"
#include "ThirdPersonCharacter.h"
#include "Components/DecalComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AActor_Projectile::AActor_Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	RootComponent = SphereComponent;
	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform); //AttachTo();

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AActor_Projectile::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) {
		OnRep_UpdateDirection();
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("%f , %f , %f, "), MoveDirection.X, MoveDirection.Y, MoveDirection.Z));

	}

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AActor_Projectile::OnOverlapBegin);

	SphereComponent->OnComponentHit.AddDynamic(this, &AActor_Projectile::OnHit);
}

void AActor_Projectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AActor_Projectile, Damage);
	DOREPLIFETIME(AActor_Projectile, Speed);
	DOREPLIFETIME(AActor_Projectile, ProjectileType);
	DOREPLIFETIME(AActor_Projectile, MoveDirection);
	DOREPLIFETIME(AActor_Projectile, Team);
}

// Called every frame
void AActor_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bendMove) {
		SetActorLocationOnServer(MoveDirection);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT(" vec ")));

		bendMove = true;
	}

	//if (FVector::Distance(GetActorLocation(), MoveDirection) > 10) {
	//	SetActorLocationOnServer(vec);
	//	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT(" arrive ")));
	//}
	//else {
	//	SetActorLocationOnServer(FVector::ZeroVector);
	//}

	if (HasAuthority()) {
		if (LifeTime > 0.0f) {
			LifeTime -= DeltaTime;
		}
		else {
			DestroyObjectOnServer();
		}
	}
}

void AActor_Projectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this) {
		if (OtherActor->ActorHasTag("Player")) {
			HitOnServer(OtherActor);
		}
		else if (!OtherActor->ActorHasTag("Gun")) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Hitt : ")) + OtherActor->GetName());
			DestroyObjectOnServer();
		}



		/*FRotator rot;
		rot.Pitch += 90;

		auto Decal = GetWorld()->SpawnActor<ADecalActor>(GetActorLocation(), rot);
		if (Decal) {
			Decal->SetDecalMaterial(Decal_Bullethole);
			Decal->SetLifeSpan(5.0f);
			Decal->GetDecal()->DecalSize = FVector(15.0f, 15.0f, 15.0f);
		}*/

		DestroyObjectOnServer();
	}

}

void AActor_Projectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{

	if (OtherActor->ActorHasTag("Gun")) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("OnHit : ")) + OtherActor->GetName());

	/*auto Target = Cast<AThirdPersonCharacter>(OtherActor);

	if (Target) {
		if (Target->GetTeam() != Team) {
			UGameplayStatics::ApplyDamage(OtherActor, Damage, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, UDamageType::StaticClass());
		}
	}*/



	SpawnBulletHole(OtherComponent, Hit.ImpactPoint, Hit.ImpactNormal);
}

void AActor_Projectile::Move(FVector dir)
{
	SetActorLocationOnServer(dir);
}

void AActor_Projectile::OnRep_UpdateDirection()
{
	//MoveDirection = _dir;
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, GetName() + FString::Printf(TEXT(" :  %f , %f , %f, "), MoveDirection.X, MoveDirection.Y, MoveDirection.Z));

}

void AActor_Projectile::OnRep_UpdateDamage(float _damage)
{
	Damage = _damage;
}

void AActor_Projectile::OnRep_UpdateSpeed(float _speed)
{
	Speed = _speed;
}

void AActor_Projectile::OnRep_UpdateType(EProjectileEnum type)
{
	ProjectileType = type;
}

void AActor_Projectile::SetDirection(FVector _dir)
{
	MoveDirection = _dir;
	OnRep_UpdateDirection();
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

	//DestroyObjectOnServer();
}

void AActor_Projectile::Setloc(FVector loc)
{
	SetActorLocation(loc);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, GetName() + FString::Printf(TEXT(" :  %f , %f , %f, "), loc.X, loc.Y, loc.Z));

	//SetDirectionOnServer(loc);
	//SetDirectionOnClient(loc);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, GetName() + FString::Printf(TEXT(" :  %f , %f , %f, "), MoveDirection.X, MoveDirection.Y, MoveDirection.Z));

}

void AActor_Projectile::OnRep_UpdateDir()
{

}

void AActor_Projectile::SpawnDamage(float _damage)
{
	SpawnNiagaraOnMulti();
	FString dmg = FString::FormatAsNumber((int)_damage);
	for (char c : dmg) {
		switch (c)
		{
		case '0':
			SpawnTextureOnMulti(0);
			break;
		case '1':
			SpawnTextureOnMulti(1);
			break;
		case '2':
			SpawnTextureOnMulti(2);
			break;
		case '3':
			SpawnTextureOnMulti(3);
			break;
		case '4':
			SpawnTextureOnMulti(4);
			break;
		case '5':
			SpawnTextureOnMulti(5);
			break;
		case '6':
			SpawnTextureOnMulti(6);
			break;
		case '7':
			SpawnTextureOnMulti(7);
			break;
		case '8':
			SpawnTextureOnMulti(8);
			break;
		case '9':
			SpawnTextureOnMulti(9);
			break;
		default:
			break;
		}
	}



}

void AActor_Projectile::SpawnTextureOnMulti_Implementation(int32 idx)
{
	if (NiagaraComp) {
		//UNiagaraFunctionLibrary::SetTextureObject(NiagaraComp, FString("user.Digit1"), DamageFonts[idx]);
	}
}

void AActor_Projectile::SpawnNiagaraOnMulti_Implementation()
{
	NiagaraComp = NewObject<UNiagaraComponent>(this);
	if (NiagaraComp)
	{
		if (NiagaraFX)
		{
			FVector SpawnLocation = GetActorLocation() + FVector(0, 30.f, 0);

			NiagaraComp->SetAsset(NiagaraFX);
			NiagaraComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			NiagaraComp->SetRelativeLocation(SpawnLocation);
			NiagaraComp->RegisterComponent();
			NiagaraComp->Activate(true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NiagaraSystem load failed!"));
		}
	}

}

void AActor_Projectile::ShootOnClient_Implementation(FVector dir)
{
	MoveDirection = (MoveDirection - GetActorLocation()).GetSafeNormal();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("%f , %f , %f, "), MoveDirection.X, MoveDirection.Y, MoveDirection.Z));



	//SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AActor_Projectile::OnOverlapBegin);

	ProjectileMovement->AddForce(FVector::ZeroVector);
}

void AActor_Projectile::ShootOnServer_Implementation(FVector dir)
{
	ShootOnClient(dir);
}
