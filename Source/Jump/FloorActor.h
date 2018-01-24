// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorActor.generated.h"

UCLASS()
class JUMP_API AFloorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// ����/��ʾ Actor
	UFUNCTION()
		void DisableActor(bool disable);

	// ���� Actor
	UFUNCTION()
		void ShowActor();
	
	// ���� Actor
	UFUNCTION()
		void HideActor();

	// ����ʱ��
	UPROPERTY(EditAnywhere , Category = "Show")
		float showTime;//��������

	// ��Z��ƫ��λ�ó���
	UPROPERTY(EditAnywhere , Category = "Show")
		float ZFromShow;


	// �����ƶ��ٶ�(��С�Լ�����)
	UPROPERTY(EditAnywhere, Category = "Idle")
		FVector IdleMoveSpeed;

	// �Ƿ���п����ƶ�
	UPROPERTY(EditAnywhere, Category = "Idle")
		bool CanIdleMove;

	// Floor�ķ���
	UPROPERTY(EditAnywhere, Category = "Score")
		int score;
	
	float showSpeed;//�����ٶ�
	float remainShowTime;//ʣ��ĳ���ʱ��
	float remainHideTime;//ʣ�������ʱ��
	bool isIdleMove;//�Ƿ���Կ�ʼ�����ƶ�
	float IdleRunTime;//�����ƶ���ʼ���ʱ��

	
};
