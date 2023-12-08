# 프로젝트 명 : MultiFPS

### UnrealEngine4의 Dedicated Server를 통한 MultiPlay 환경 FPS 게임입니다.
## 동영상
https://youtu.be/_6Vk6LrY_8Q
[![IU(아이유) _ Into the I-LAND](http://img.youtube.com/vi/QYNwbZHmh8g/0.jpg)](https://youtu.be/QYNwbZHmh8g?t=0s) 
## 이미지

![스크린샷 2023-11-30 215702](https://github.com/joyeonguns/ue4-multi-project/assets/85017198/b375218c-81d1-4e36-9c82-57556e30beb0)

## 클래스에 관한정보
<br/>

## 1. 캐릭터 클래스

**ThirdPersonCharacter** 

    플레이어 캐릭터 클래스로 이동, 점프, 슬라이딩, 줌 등의 키입력을 통한 조작기능
    레이케스트를 통한 상호작용 오브젝트 검출      
    줌 상태와 일반상태의 카메라 및 하위오브젝트 상태 전환                
  
<br/>

## 2. 플레이어 컨트롤러 클래스

**MyPlayerController** 

    각종 Userwidget의 생성 및 제거, 접근 제어 

<br/>

## 3. ActorComponent 클래스

**Character_State_Component** 

    ThirdPersonCharacter 클래스에 부착되는 컴포넌트
    캐릭터의 쉴드와 체력을 관리 및 PlayerController를 통해 Userwidget에 반영

**Charater_SKill_Component**

    ThirdPersonCharacter 클래스에 부착되는 컴포넌트
    캐릭터의 스킬 쿨다운을 관리 및 PlayerController를 통해 Userwidget에 반영

**ActorComponent_FloatingDamage**

    ThirdPersonCharacter 클래스에 부착되는 컴포넌트
    데미지 텍스트(Userwidget)를 Viewport에 띄우는데 사용
    해당 플레이어의 클라이언트에서만 데미지 텍스트를 띄우기 위해 캐릭터클래스에 부착

**Character_State_Component**

    해당 플레이어의 클라이언트에서만 적용하기위해 캐릭터클래스에 부착
    Camerashake 클래스를 통해 플레이어의 카메라를 흔들어줌
    

<br/>

## 4. Actor 클래스

**MyGunActor**

    플레이어가 사용할 총 클래스, fire 함수를 통해 서버에 투사체를 생성
    Gundata 구조체를 통해 데이터를 관리
    Gundata의 Type이 Auto일 경우 Timeline을 통해 총의 반동기능을 실행    

**Actor_Projectile** 

    MyGunActor의 fire함수를 통해 생성되는 오브젝트
    투사체 생성시 발사 사운드 재생 및 ProjectileMovement를 통해 빠른속도로 이동
    빠른 속도로 발사하는 투사체가 오브젝트의 충돌을 감지하기위해 실제 Mesh보다 훨씬 큰 범위를 RayCast를 통해 검출
    RayCast로 오브젝트가 검출될 경우 OnHit 함수 실행 및 충돌된 위치에 Decal생성을 통해 총알 자국 생성
    충돌된 오브젝트가 다른팀 플레이어 or 훈련봇일 데미지텍스트 생성 및 데미지적용
    충돌한 플레이어의 경우 CameraShake에임펀치

<br/>    

## 5. CameraShake 클래스

**MyMatineeCameraShake**

    플레이어의 카메라를 흔들어줌

<br/>

## 6. UserWidget 클래스

**UserWidget_ChatLogs** 

    플레이어가 입력한 채팅을 입력시 생성되어 UserWidget_ChatUI를 통해 모든 클라이언트에 보여줌

**UserWidget_ChatUI**

    생성된 UserWidget_ChatLogs 가 저장및 플레이어에게 보여줌

**UserWidget_Crosshair**

    쉴드, 체력, 스킬 쿨타임, 크로스헤어 등을 보여주는 전투 UI

**UserWidget_DamageText**

    ActorComponent_FloatingDamag를 통해 데미지텍스트를 보여줌
    Userwidget의 Animation 기능을 이용하여 텍스트에 효과를 줌    
    
**UserWidget_Dead**

    캐릭터가 사망시 나오는 Userwidget, 버튼을 눌러 리스폰

**UserWidget_TeamSelecUI**

    게임시작시 생성되는 Userwidget Red팀 Blue팀 중 선택

**UserWidget_TrainingSettingMenu.**

    게임의 메뉴 Userwidget
    기본 메뉴에서 해상도 변경, 마우스 감도, 팀 선택, 사운드 크기 등을 제어 
    훈련 메뉴에서 이동 타겟의 속도, 에임 테스트의 난이도 제어

Developed with Unreal Engine 4
