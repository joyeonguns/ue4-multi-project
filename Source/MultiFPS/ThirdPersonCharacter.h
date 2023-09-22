// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Actor_Projectile.h"
#include <Components/SceneComponent.h>
#include <Components/SceneCaptureComponent2D.h>
#include "Engine/CanvasRenderTarget2D.h"
#include "ThirdPersonCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnUpdateHPDelecate);

UENUM(BlueprintType)
enum class EStateEnum : uint8
{
	Zoom UMETA(DisplayName = "LightShot"),
	Hipfire UMETA(DisplayName = "HeavyShot"),
	Run UMETA(DisplayName = "SniperShot"),
};


UCLASS()
class MULTIFPS_API AThirdPersonCharacter : public ACharacter //, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AThirdPersonCharacter();

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/*UPROPERTY(EditAnywhere, Category = SpwnActor)
		TSubclassOf<AActor_Projectile> SpwActorClass;*/

	UPROPERTY(EditAnywhere, Category = SpwnActor)
		TSubclassOf<AActor_Projectile> SpwProjectileClass;

	//UPROPERTY(EditAnywhere, Category = SpwnActor)
	//	TSubclassOf<ATestProject1Projectile> ProjectileClass;

	// FirstPerson
	UPROPERTY(EditAnywhere, Category = FP_Mesh)
		class USkeletalMeshComponent* FP_Arms;

	UPROPERTY(EditAnywhere, Category = FP_Camera)
		class UCameraComponent* FP_Camera;

	UPROPERTY(EditAnywhere, Category = FP_Gun)
		class USkeletalMeshComponent* FP_Gun;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = SpwnActor)
		class USceneComponent* ShotPoint;


	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMesh* sk_Mesh;

	UPROPERTY(EditAnywhere, Category = SceneCapture)
		class USceneCaptureComponent2D* ScopeCamera;

	UPROPERTY(EditAnywhere, Category = SceneCapture)
		class UCanvasRenderTarget2D* ScopeRenderImage;

	UPROPERTY(EditAnywhere, Category = Component)
		class UCharacter_State_Component* Character_State_Component;

	UPROPERTY(EditAnywhere, Category = Component)
		class UCharater_SKill_Component* Character_Skill_Component;

	UPROPERTY(EditAnywhere, Category = Materials)
		TArray<UMaterial*> Mats;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	FOnUpdateHPDelecate OnUpdateHP;

	// INPUT
	void InputRun();

	void InputInteraction();

	void InputJump();

	void InputSit();

	// 
	void InputFire();

	void Fire();

	void InputFireEnd();
	UFUNCTION(unreliable, server)
		void FireEndOnServer();
	UFUNCTION(unreliable, netmulticast)
		void FireEndOnMulti();

	bool bRun = false;

	// 
	void Vaulting(float DeltaTime);

	void CheckVaulting();

	// 
	void Sliding();

	void Hit();

	// 
	void ChangeCamera();

	// 
	void SearchInteractionObj();

	// 

	// 
	void SearchHipShotPointDirection();

	// 
	void SearchAimShotPointDirection();

	UFUNCTION(reliable, server)
		void SearchAimOnServer(FTransform tp);

	UFUNCTION(reliable, netmulticast)
		void SearchAimOnMulti(FTransform tp);

	// 
	void SetPitchYaw(float DeltaTime);

	UFUNCTION(BlueprintCallable)
		float Get_Pitch();

	UFUNCTION(BlueprintCallable)
		float Get_Yaw();

	void BeginSetting();

	// 
	void InputReload();
	void Reloading();
	UFUNCTION(reliable, server)
		void ReloadingOnServer();
	UFUNCTION(reliable, netmulticast)
		void ReloadingOnMulti();

	void ArmoClear();

	void ArmoFill();

	// 
	void SetZoom(bool _zoom);

	void SetScopeTarget();

	void SetSetting();

	void BindFloatingHP();

	void InputZoom();

	void Zooming(float DeltaTime);

	UFUNCTION(reliable, netmulticast)
		void ZoomOff();


	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		EStateEnum _playerState = EStateEnum::Hipfire;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Replicated)
		bool Zoom;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Replicated)
		bool OnlyBodyAnim;

public:
	// 

	// 
	UFUNCTION(reliable, server)
		void SwitchRunOnServer();

	UFUNCTION(reliable, netmulticast)
		void SwitchRunOnMulti();

	// 
	UFUNCTION(reliable, server)
		void SwitchWalkOnMulti();
	UFUNCTION(reliable, netmulticast)
		void SwitchWalkOnServer();


	// 
	UFUNCTION(unreliable, netmulticast)
		void PlayAnimMontageOnClinet(UAnimMontage* _AnimMontage);

	UFUNCTION(unreliable, server)
		void PlayAnimMontageOnServer(UAnimMontage* _AnimMontage);

	// 
	UFUNCTION(reliable, server)
		void SetActorLocationOnServer(FVector newLocation);
	UFUNCTION(reliable, netmulticast)
		void SetActorLocationOnMulti(FVector newLocation);

	// 
	UFUNCTION(reliable, server)
		void SetGravityScaleOnServer(float _scale);
	UFUNCTION(reliable, netmulticast)
		void SetGravityScaleOnClient(float _scale);

	// 
	UFUNCTION(unreliable, server)
		void HiddeFirstPersonOnServer();

	// 
	UFUNCTION(unreliable, server)
		void SetRotOnServer(FRotator rot);
	UFUNCTION(unreliable, netmulticast)
		void SetRotOnMulti(FRotator rot);

	//
	void Equip2();

	UFUNCTION(unreliable, server)
		void Equip2OnServer();

	UFUNCTION(unreliable, netmulticast)
		void Equip2OnMulti();

	// 
	void Equip_FirstGun();

	UFUNCTION(unreliable, server)
		void Equip_FirstGunOnServer();

	UFUNCTION(unreliable, netmulticast)
		void Equip_FirstGunOnMulti();

	//
	void Equip_SecondGun();

	UFUNCTION(unreliable, server)
		void Equip_SecondGunOnServer();

	UFUNCTION(unreliable, netmulticast)
		void Equip_SecondOnMulti();

	//
	void ThrowGun();
	UFUNCTION(unreliable, server)
		void ThrowGunOnServer();
	UFUNCTION(unreliable, netmulticast)
		void ThrowGunOnMulti();

	//
	UFUNCTION(unreliable, server)
		void InputFireOnServer();

	UFUNCTION(unreliable, netmulticast)
		void InputFireOnMulti();


	// 
	UFUNCTION()
		void OnRep_UpdatedEquipGun();

	UFUNCTION()
		void OnRep_UpdatedSearchGun();

	// Set SearchGun
	UFUNCTION(unreliable, server)
		void SearchGunOnServer(class AMyGunActor* hitActor);

	UFUNCTION(unreliable, netmulticast)
		void SearchGunOnMulti(class AMyGunActor* hitActor);


	UFUNCTION(unreliable, server)
		void SetPitchYawOnServer(float DeltaTime);

	UFUNCTION()
		void OnRep_UpdatedAimDirection();

	UFUNCTION(unreliable, server)
		void SetZoomOnServer(bool _zoom);

	UFUNCTION(unreliable, server)
		void SetFireSyncTransformOnServer(FTransform transform);

	UFUNCTION()
		void OnRep_UpdatedStatSetting();

	float Get_HpRatio();

	// 
	ETeamEnum GetTeam();

	void SetTeam(ETeamEnum newTeam);

	void ApplyTeam(ETeamEnum newTeam);

	UFUNCTION(unreliable, server)
		void SetTeamOnServer(ETeamEnum newTeam);

	UFUNCTION(unreliable, netmulticast)
		void SetTeamOnMulti(ETeamEnum newTeam);

	// skill
	void InputSkill0();
	void InputSkill1();

	// gun Select
	void InputSelectGun_0();

	void ChangeGun_0();

	void SelectGun_0();
	UFUNCTION(unreliable, netmulticast)
		void SelectGun_0OnMulti();
	UFUNCTION(unreliable, server)
		void SelectGun_0OnServer();

	void InputSelectGun_1();

	void ChangeGun_1();
	void SelectGun_1();
	UFUNCTION(unreliable, netmulticast)
		void SelectGun_1OnMulti();
	UFUNCTION(unreliable, server)
		void SelectGun_1OnServer();

	UPROPERTY(ReplicatedUsing = OnRep_UpdatedAimDirection, EditAnywhere, Category = Gun)
		FVector HipShotPoint;

	UPROPERTY(Replicated, EditAnywhere, Category = Gun)
		FVector AimShotPoint;

private:
	class AMyPlayerController* plController;

	UPROPERTY(EditAnywhere, Category = Anim)
		UAnimMontage* Anim_DeepVault;

	UPROPERTY(EditAnywhere, Category = Anim)
		UAnimMontage* Anim_Sliding;

	UPROPERTY(EditAnywhere, Category = Anim)
		UAnimMontage* Anim_Jump;

	UPROPERTY(EditAnywhere, Category = Anim)
		UAnimMontage* Anim_Reload;

	UPROPERTY(EditAnywhere, Category = Anim)
		UAnimMontage* Anim_Death;

	UPROPERTY(EditAnywhere, Category = Anim)
		UAnimMontage* Anim_Zoom;

	UPROPERTY(EditAnywhere, Category = Anim)
		UAnimMontage* Anim_ChangeGun;

	UPROPERTY(Replicated, EditAnywhere, Category = Gun)
		int32 curGun;

	UPROPERTY(ReplicatedUsing = OnRep_UpdatedEquipGun, EditAnywhere, Category = Gun)
		TArray<TWeakObjectPtr<class AMyGunActor>> EquipedGuns = { nullptr, nullptr };

	UPROPERTY(ReplicatedUsing = OnRep_UpdatedSearchGun, EditAnywhere, Category = Gun)
		class AMyGunActor* SearchGun;

	UPROPERTY(EditAnywhere, Category = UI)
		class UWidgetComponent* floatingHP;

	UPROPERTY(Replicated, EditAnywhere, Category = Team)
		ETeamEnum MyTeam;


	bool bVaulting = false;
	bool bCanVault = false;

	FVector VaultEndLoc;
	FVector VaultStartLoc;

	FVector VaultTargetLoc;

	float VaultTime = 0.0;

	bool b = false;

	bool bFirstPersonMode;

	class AActor* InteractionOBJ;

	bool findWeapon = false;

	bool bReloading = false;


	FTimerHandle TH_Fire;

	UPROPERTY(Replicated)
		float Aim_Pitch;

	UPROPERTY(Replicated)
		float Aim_Yaw;

	FTransform SyncTransform;

	class UAnimInstance_Player* anim;

	bool bzooming;
	float ZoomTime;
	bool bChangeGun;
	FVector ThirdPersonCameraLoc;


	FTransform CamTransform;
};
