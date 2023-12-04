# MultiFPS

# UnrealEngine4의 Dedicated Server를 통한 MultiPlay 환경 FPS 게임입니다.

![스크린샷 2023-11-30 215702](https://github.com/joyeonguns/ue4-multi-project/assets/85017198/b375218c-81d1-4e36-9c82-57556e30beb0)

## 클래스에 관한정보

### 캐릭터 클래스

**ThirdPersonCharacter** 

    플래이어 캐릭터 클래스로 이동, 점프, 슬라이딩, 줌 등의 키입력을 통한 조작기능
    레이케스트를 통한 상호작용 오브젝트 검출  
    
    줌 상태와 일반상태의 카메라 및 하위오브젝트 상태 전환                
  

### 플레이어 컨트롤러 클래스

**MyPlayerController** 
    각종 Userwidget의 생성 및 제거, 접근 제어 

### ActorComponent 클래스

**Character_State_Component** 

    ThirdPersonCharacter 클래스에 부착되는 컴포넌트
    캐릭터의 쉴드와 체력을 관리 및 PlayerController를 통해 Userwidget에 반영

**Charater_SKill_Component**

    ThirdPersonCharacter 클래스에 부착되는 컴포넌트
    캐릭터의 스킬 쿨다운을 관리 및 PlayerController를 통해 Userwidget에 반영

**ActorComponent_FloatingDamage**

    ThirdPersonCharacter 클래스에 부착되는 컴포넌트
    데미지 텍스트(Userwidget)를 Viewport에 띄우는데 사용한다.
    해당 플레이어의 클라이언트에서만 데미지 텍스트를 띄우기 위해 캐릭터클래스에 부착한다.


### Actor 클래스

**MyGunActor**

    플레이어가 사용할 총 클래스
    

**Actor_Projectile** 
    
    

## CameraShake 클래스

**MyMatineeCameraShake**

### UserWidget 클래스

**UserWidget_ChatLogs** 

**UserWidget_ChatUI**

**UserWidget_Crosshair**

**UserWidget_DamageText**

**UserWidget_Dead**

**UserWidget_TeamSelecUI**

**UserWidget_TrainingSettingMenu.**

Developed with Unreal Engine 4
