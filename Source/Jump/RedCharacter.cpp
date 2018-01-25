// Fill out your copyright notice in the Description page of Project Settings.

#include "RedCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Engine.h>
#include "FloorActor.h"

// Sets default values
ARedCharacter::ARedCharacter()
{

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 使用弹簧臂让摄像机运动平稳而自然。
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->AttachTo(GetCapsuleComponent());
	SpringArm->RelativeRotation = FRotator(-10.0f, 20.0f, 0.f);
	SpringArm->RelativeLocation = FVector(0.0f, 0.0f, 40.0f);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 1.0f;
	SpringArm->CameraLagSpeed = 3.0f;

	// 创建一个摄像机，将其附着到弹簧臂
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);




	// 初始化变量
	setJumpTime(0);
	onReadyJump = false;//蓄力跳跃的状态
    onPauseJump = false;//暂停蓄力跳跃的状态（保留蓄力)
	onFullPower = false;//满蓄力状态
	isDead = false;
	score = 0;
	

	// floorArray
	TSubclassOf<AFloorActor> classToFind;
	classToFind = AFloorActor::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, floorsArray);
	//按floor名称排序
	floorsArray.Sort([](const AActor & A, const AActor & B) {
		return A.GetName() < B.GetName();
	});
	cur_floor = 0;
	canChangeDirection = false;



	// 控制器禁止控制方向
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	
}

// Called when the game starts or when spawned
void ARedCharacter::BeginPlay()
{

	Super::BeginPlay();

	// 先禁用所有floors
	for (int i = 0; i < floorsArray.Num(); i++) {

		AFloorActor * floor = Cast<AFloorActor>(floorsArray[i]);
		floor->DisableActor(true);

	}

	// 显示 0 ， 1 floor
	if (floorsArray.Num() > 0) {

		AFloorActor * C_floor = Cast<AFloorActor>(floorsArray[cur_floor]);
		AFloorActor * N_floor = Cast<AFloorActor>(floorsArray[getNextFloor()]);
		C_floor->DisableActor(false);
		N_floor->ShowActor();

	}

	
}

// Called every frame
void ARedCharacter::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	//跳跃蓄力计时
	if (onReadyJump) {

		//不能是暂停状态
		if (!onPauseJump) {


			jumpTime += DeltaTime;

			//满蓄力驱动
			if (jumpTime > maxJumpTime && !onFullPower) {

				onFullPower = true;
				FullPowerDrive();

			}

			setJumpTime(FMath::Min(jumpTime, maxJumpTime));//不能超过最大时间


		}

	}

	// 显示调试信息
	if (GEngine)
	{

		FString message("Power Time : ");
	
		message += FString::SanitizeFloat(jumpTime);
		message += " \n Score : " + FString::SanitizeFloat(score);
		message += " \n Floor Index : " + FString::SanitizeFloat(cur_floor);
		message += " \n Floor Name : ";
	

		if (floorsArray.Num() > 0) {

			//块总数
			message += (floorsArray[cur_floor]->GetName());
			message += " \n Num : " + FString::SanitizeFloat(floorsArray.Num());

			//message += " \n Origin: " + origin.ToString();
			//message += " \n Box: " + box.ToString();
			//获取当前floor的边界

			FVector origin;
			FVector box;
			floorsArray[cur_floor]->GetActorBounds(false, origin, box);
			FVector2D minPoint(origin.X - box.X , origin.Y - box.Y );
			FVector2D maxPoint(origin.X + box.X , origin.Y + box.Y );
			FBox2D floorBox(minPoint, maxPoint);

			//player Box
			FVector playerOrigin = GetActorLocation();
			float radius = GetCapsuleComponent()->GetScaledCapsuleRadius();
			FVector2D playerMinPoint(playerOrigin.X - radius , playerOrigin.Y - radius);
			FVector2D playerMaxPoint(playerOrigin.X + radius, playerOrigin.Y + radius);
			FBox2D playerBox(playerMinPoint, playerMaxPoint);

			/*message += " \n Player Origin: " + playerOrigin.ToString();
			message += " \n Player Box: " + playerBox.ToString();
			message += " \n Floor Origin: " + origin.ToString();
			message += " \n Floor Box: " + floorBox.ToString();*/
			
		}
		
		GEngine->AddOnScreenDebugMessage(0, 60.0f, FColor::Green, (message), true, FVector2D(1, 1));

	}

	// 当前地板转换及方向转换判断
	if (GetCharacterMovement()->GetMovementName() != "Falling" && canChangeDirection) {

		canChangeDirection = false;

		// 判断是不是到新的floor上, 如果是则改变方向
		if (floorsArray.Num() > 0) {

			//获取当前floor的边界
			FVector origin;
			FVector box;
			floorsArray[cur_floor]->GetActorBounds(false, origin, box);
			
			FVector2D minPoint(origin.X - box.X , origin.Y - box.Y );
			FVector2D maxPoint(origin.X + box.X , origin.Y + box.Y );
			FBox2D floorBox(minPoint , maxPoint);
			
			//获取player的边界
			FVector playerOrigin = GetActorLocation();
			float radius = GetCapsuleComponent()->GetScaledCapsuleRadius() + 20;
			FVector2D playerMinPoint(playerOrigin.X - radius, playerOrigin.Y - radius);
			FVector2D playerMaxPoint(playerOrigin.X + radius, playerOrigin.Y + radius);
			FBox2D playerBox(playerMinPoint, playerMaxPoint);

			if (!floorBox.Intersect(playerBox) && !floorBox.IsInside(playerBox)) {

				// 开始改变方向
				changeDirection();

			}
			else {
				// 未到达新的块
				EndJumpDrive();//结束跳跃事件驱动

			}
		}

	}

	// 判断是否游戏结束
	if (GetCharacterMovement()->GetMovementName() == "Falling") {

		if (floorsArray.Num() > 0) {

			//获取当前floor的边界
			FVector origin;
			FVector box;
			float cur_maxZ;//当前块的Z轴边界坐标
			float next_maxZ;//下一块的Z轴周边界坐标
			float floor_minZ;//所有floor最小Z轴边界坐标
			int next_floor = getNextFloor();//下一块floor的下标
			floorsArray[cur_floor]->GetActorBounds(false, origin, box);
			cur_maxZ = origin.Z + box.Z;
			floorsArray[next_floor]->GetActorBounds(false, origin, box);
			next_maxZ = origin.Z + box.Z;
			floor_minZ = FMath::Min(cur_maxZ, next_maxZ);//所有floor最大Z轴边界坐标


			//获取player的边界
			float player_minZ;//当前玩家最小Z轴坐标
			FVector playerOrigin = GetActorLocation();
			float radius = GetCapsuleComponent()->GetScaledCapsuleRadius() + 5;
			player_minZ = playerOrigin.Z - radius;


			//判断游戏是否结束
			if (player_minZ + GameOverZOffset < floor_minZ) {

				//reStartLevel();重新开始关卡
				GameOverDrive();

			}

			//判断是否人物死亡
			if ((!isDead) && (player_minZ + 30 < floor_minZ) ) {

				isDead = true;
				SpringArm->CameraLagSpeed = 6.0f;
				DieDrive();

			}

		}

	}


	// 按改变方向的速度及剩余时间改变方向
	if (remainTimeOfChange > 0) {

		SetActorRotation(GetActorRotation() + velocityOfChange * DeltaTime);
		remainTimeOfChange -= DeltaTime;
	
	}


}

// Called to bind functionality to input
void ARedCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 设置“移动”绑定。
	InputComponent->BindAxis("MoveForward", this, &ARedCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARedCharacter::MoveRight);

	// 设置“查看”绑定。
	InputComponent->BindAxis("Turn", this, &ARedCharacter::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &ARedCharacter::AddControllerPitchInput);

	// 跳远状态
	InputComponent->BindAction("Jump", IE_Pressed, this, &ARedCharacter::ReadyJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ARedCharacter::StartJump);

	// 暂停
	FInputActionBinding& toggle = InputComponent->BindAction("Pause", IE_Pressed, this, &ARedCharacter::setPause);
	toggle.bExecuteWhenPaused = true; //暂停时可以执行

	// 重新开始
	InputComponent->BindAction("ReStart", IE_Pressed, this, &ARedCharacter::reStartLevel);

}


void ARedCharacter::MoveForward(float Value)
{
	// 明确哪个方向是“前进”，并记录玩家试图向此方向移动。
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void ARedCharacter::MoveRight(float Value)
{
	// 明确哪个方向是“向右”，并记录玩家试图向此方向移动。
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}


void ARedCharacter::ReadyJump()
{
	//非空中状态才可以准备跳跃
	if (GetCharacterMovement()->GetMovementName() != "Falling" && !onReadyJump && !onPauseJump) {

		
		setJumpTime(0);//开始统计时间
		onReadyJump = true;//跳跃状态开始
		ReadyJumpDrive();//准备跳跃事件驱动

		onFullPower = false;//满蓄力状态

	}


}

void ARedCharacter::StartJump()
{
	
	setJumpSpeed(this->speed * jumpTime);//设置起跳速度
	bPressedJump = true;//启动起跳

	onReadyJump = false;//结束跳跃状态
	setJumpTime(0);//重置跳跃统计时间
	onPauseJump = false;//结束暂停跳跃的状态
	onFullPower = false;//满蓄力状态

	canChangeDirection = true;
	StartJumpDrive();//开始跳跃事件驱动
	
}

void ARedCharacter::cancelJump() {


	setJumpTime(0);//开始统计时间
	onReadyJump = false;//跳跃状态开始
	onPauseJump = false;//结束暂停跳跃的状态
	onFullPower = false;//满蓄力状态


}

void ARedCharacter::pauseJump() {

	onPauseJump = true;
}

void ARedCharacter::ResumeJump() {

	onPauseJump = false;

}

void ARedCharacter::setJumpSpeed(float speed) {

	// 设置跳前速度
	UCharacterMovementComponent * characterMove = GetCharacterMovement();
	FVector  forwardVector = GetActorForwardVector();
	forwardVector.Normalize();
	FVector newVelocity = forwardVector * speed;
	characterMove->Velocity.X = newVelocity.X;
	characterMove->Velocity.Y = newVelocity.Y;

	// 垂直速度
	float vertiSpeed = speed * FMath::Sin(FMath::DegreesToRadians(60));
	characterMove->JumpZVelocity = FMath::Max(vertiSpeed, minVertiSpeed);


}

void ARedCharacter::setPause() {

	// 暂停转换
	UGameplayStatics::SetGamePaused(GetWorld(), !UGameplayStatics::IsGamePaused(GetWorld()));

}

void ARedCharacter::reStartLevel() {


	// 设置初始位置与方向
	FVector InitLocation(0, 0, 112);
	FRotator InitRotation(0, 0, 0);
	SetActorLocation(InitLocation);
	SetActorRotation(InitRotation);
	
	onReadyJump = false;
	onPauseJump = false;
	cur_floor = 0;
	setJumpTime(0);
	canChangeDirection = false;
	score = 0;

}


void ARedCharacter::changeDirection() {

	if (floorsArray.Num() > 0) {

		
		// 切换floor
		cur_floor = getNextFloor();
		int next_floor = getNextFloor();

		//Calculates the lookat rotation from direction vector
		FVector direction = floorsArray[next_floor]->GetActorLocation() - GetActorLocation();
		FRotator lookAtRotator = FRotationMatrix::MakeFromX(direction).Rotator();



		// 设置开始改变方向的参数
		//velocityOfChange = (lookAtRotator - GetActorRotation()) * (1 / timeOfChange);//改变方向的速度
		//remainTimeOfChange = timeOfChange;// 剩余时间
		SetActorRotation(lookAtRotator);


		// 玩家分数增加
		addScore();


		// 激活下一个floor， 隐藏上一个floor
		AFloorActor * L_floor = Cast<AFloorActor>(floorsArray[getLastFloor()]);
		AFloorActor * N_floor = Cast<AFloorActor>(floorsArray[getNextFloor()]);
		L_floor->HideActor();//隐藏上一个floor
		N_floor->ShowActor();//显示下一个floor


		// 成功闯关驱动
		if (cur_floor == floorsArray.Num()) {

			VictoryDrive();

		}

	}
}


int ARedCharacter::getNextFloor() {

	int next_floor = cur_floor + 1;
	if (next_floor >= floorsArray.Num()) {
		next_floor = 0;
	}

	return next_floor;

}


int ARedCharacter::getLastFloor() {

	int last_floor = cur_floor - 1;
	if (last_floor < 0 ) {
		last_floor = floorsArray.Num() - 1;
	}

	return last_floor;
}


void ARedCharacter::addScore() {


	AFloorActor * floor = Cast<AFloorActor>(floorsArray[cur_floor]);

	//获取当前floor的2D中心点
	FVector floorPos = floor->GetActorLocation();
	FVector2D floorPos2D = FVector2D(floorPos.X, floorPos.Y);
	
	//获取当前player的2D中心点
	FVector playerPos = GetActorLocation();
	FVector2D playerPos2D = FVector2D(playerPos.X, playerPos.Y);

	if (FVector2D::Distance(floorPos2D, playerPos2D) < doubleScoreOffset) {

		score += 2*(floor->score);
		//增加双倍分数驱动事件
		AddDoubleScoreDrive();

	}
	else {
		score += (floor->score);
		//增加分数驱动事件
		AddScoreDrive();
	}

}

void ARedCharacter::setJumpTime(float time) {

	jumpTime = time;
	power = jumpTime / maxJumpTime;

}