// Fill out your copyright notice in the Description page of Project Settings.

#include "FloorActor.h"


// Sets default values
AFloorActor::AFloorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//��ʼ������
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

	//����Actor��ʣ��ʱ��
	if (remainShowTime > 0) {

		remainShowTime -= DeltaTime;

		// ���Կ�ʼ�����ƶ�
		if (remainShowTime <= 0) {
			isIdleMove = true;
		}

		// �µ�λ��
		FVector curLocation = GetActorLocation();
		FVector offsetLocation = DeltaTime * showSpeed * FVector(0, 0, 1);
		SetActorLocation(curLocation + offsetLocation);
	}

	//����Actr��ʣ��ʱ��
	if (remainHideTime > 0) {

		remainHideTime -= DeltaTime;

		// ����Disable Floor
		if (remainHideTime <= 0) {
			DisableActor(true);
		}

		// �µ�λ��
		FVector curLocation = GetActorLocation();
		FVector offsetLocation = DeltaTime * showSpeed * FVector(0, 0, 1);
		SetActorLocation(curLocation + offsetLocation);

	}


	//�����ƶ�
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

	//����
	DisableActor(false);

	//���ó��ֳ�ʼλ��
	FVector initLocation = GetActorLocation();
	FVector offsetLocation = ZFromShow * FVector(0, 0, 1);
	SetActorLocation(initLocation + offsetLocation);

	//����ʣ�����ʱ���Լ��ٶ�
	remainShowTime = showTime;
	showSpeed = -(ZFromShow / showTime);

}


void AFloorActor::HideActor() {


	//����ʣ������ʱ���Լ��ٶ�
	remainHideTime = showTime;
	showSpeed = -showSpeed;

}