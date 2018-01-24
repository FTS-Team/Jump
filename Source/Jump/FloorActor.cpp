// Fill out your copyright notice in the Description page of Project Settings.

#include "FloorActor.h"


// Sets default values
AFloorActor::AFloorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//初始化数据
	showTime = 2;
	ZFromShow = -200;
	IdleMoveSpeed = FVector(0, 0, 0);
	isIdleMove = false;
	CanIdleMove = false;
	IdleRunTime = 0;
	remainShowTime = 0;
	remainHideTime = 0;
	showSpeed = 0;
	score = 1;

}

// Called when the game starts or when spawned
void AFloorActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFloorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//呈现Actor的剩余时间
	if (remainShowTime > 0) {

		remainShowTime -= DeltaTime;

		// 可以开始空闲移动
		if (remainShowTime <= 0) {
			isIdleMove = true;
		}

		// 新的位置
		FVector curLocation = GetActorLocation();
		FVector offsetLocation = DeltaTime * showSpeed * FVector(0, 0, 1);
		SetActorLocation(curLocation + offsetLocation);
	}

	//隐藏Actr的剩余时间
	if (remainHideTime > 0) {

		remainHideTime -= DeltaTime;

		// 可以Disable Floor
		if (remainHideTime <= 0) {
			DisableActor(true);
		}

		// 新的位置
		FVector curLocation = GetActorLocation();
		FVector offsetLocation = DeltaTime * showSpeed * FVector(0, 0, 1);
		SetActorLocation(curLocation + offsetLocation);

	}


	//空闲移动
	if (isIdleMove && CanIdleMove) {

		FVector NewLocation = GetActorLocation();
		float DeltaHeight = (FMath::Sin(IdleRunTime + DeltaTime) - FMath::Sin(IdleRunTime));

		IdleRunTime += DeltaTime;
		NewLocation += DeltaHeight * IdleMoveSpeed;

		SetActorLocation(NewLocation);

	}

}

void AFloorActor::DisableActor(bool disable) {

	SetActorHiddenInGame(disable);
	SetActorEnableCollision(!disable);
	SetActorTickEnabled(!disable);

}


void AFloorActor::ShowActor() {

	//激活
	DisableActor(false);

	//设置呈现初始位置
	FVector initLocation = GetActorLocation();
	FVector offsetLocation = ZFromShow * FVector(0, 0, 1);
	SetActorLocation(initLocation + offsetLocation);

	//设置剩余呈现时间以及速度
	remainShowTime = showTime;
	showSpeed = -(ZFromShow / showTime);

}


void AFloorActor::HideActor() {


	//设置剩余隐藏时间以及速度
	remainHideTime = showTime;
	showSpeed = -showSpeed;

}