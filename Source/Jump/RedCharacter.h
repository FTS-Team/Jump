﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RedCharacter.generated.h"

UCLASS()
class JUMP_API ARedCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARedCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// 处理前进和后退的输入。
	UFUNCTION()
		void MoveForward(float Value);

	// 处理左右移动的输入。
	UFUNCTION()
		void MoveRight(float Value);


	// 按下按键时---开始蓄力
	UFUNCTION(BlueprintCallable, Category = "RedCharacter")
		void ReadyJump();

	// 取消蓄力
	UFUNCTION(BlueprintCallable, Category = "RedCharacter")
		void cancelJump();

	// 暂停蓄力
	UFUNCTION(BlueprintCallable, Category = "RedCharacter")
		void pauseJump();

	// 继续蓄力
	UFUNCTION(BlueprintCallable, Category = "RedCharacter")
		void ResumeJump();

	// 松开按键时---结束蓄力
	UFUNCTION(BlueprintCallable, Category = "RedCharacter")
		void StartJump();
	

	// 开始准备跳跃事件驱动
	UFUNCTION(BlueprintImplementableEvent)
		void ReadyJumpDrive();

	// 开始跳跃事件驱动
	UFUNCTION(BlueprintImplementableEvent)
		void StartJumpDrive();

	// 结束跳跃事件驱动
	UFUNCTION(BlueprintImplementableEvent)
		void EndJumpDrive();

	// 添加分数事件驱动
	UFUNCTION(BlueprintImplementableEvent)
		void AddScoreDrive();

	// 添加双倍分数驱动
	UFUNCTION(BlueprintImplementableEvent)
		void AddDoubleScoreDrive();

	// 满蓄力驱动事件
	UFUNCTION(BlueprintImplementableEvent)
		void FullPowerDrive();

	// 结束游戏事件驱动
	UFUNCTION(BlueprintImplementableEvent)
		void GameOverDrive();

	// 人物死亡事件驱动
	UFUNCTION(BlueprintImplementableEvent)
		void DieDrive();

	// 成功闯关事件驱动
	UFUNCTION(BlueprintImplementableEvent)
		void VictoryDrive();

	// 暂停游戏
	UFUNCTION()
		void setPause();

	// 重新开始
	UFUNCTION()
		void reStartLevel();

	// 设置起跳速度
	UFUNCTION()
		void setJumpSpeed(float speed);

	// 改变方向
	UFUNCTION()
		void changeDirection();

	// 获取下一个方块
	UFUNCTION()
		int getNextFloor();

	// 获取上一个方块
	UFUNCTION()
		int getLastFloor();


	// 按当前floor增加分数
	UFUNCTION()
	void addScore();


	// 设置当前跳跃蓄力时间
	UFUNCTION()
		void setJumpTime(float time);

	// 弹簧臂
	UPROPERTY(EditAnywhere)
		class USpringArmComponent * SpringArm;

	// 分数属性
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int score;
	// 蓄力属性
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float power;


	const int speed = 500;//水平基本速度（speed * jumpTime = 最终水平速度)
	const int GameOverZOffset = 200;//人物死亡后掉落的高度
	const int doubleScoreOffset = 15;//人物双倍得分偏差
	const float minVertiSpeed = 400;//最小垂直速度
	const float timeOfChange = 1;//改变方向所用时间
	const float maxJumpTime = 2;//最大蓄力时间


	bool onReadyJump;//正在跳跃的状态
	bool canChangeDirection;//可以开始改变方向的状态
	bool onPauseJump;//暂停蓄力跳跃的状态
	bool onFullPower;//满蓄力状态
	bool isDead;//人物死亡状态

	float jumpTime;//蓄力时间
	float remainTimeOfChange;//改变方向的剩余时间
	FRotator velocityOfChange;//改变方向的速度

	
	//地板数组
	int cur_floor;
	TArray<AActor *> floorsArray;


};
