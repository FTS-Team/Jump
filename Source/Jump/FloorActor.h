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


	// 隐藏/显示 Actor
	UFUNCTION()
		void DisableActor(bool disable);

	// 呈现 Actor
	UFUNCTION()
		void ShowActor();
	
	// 隐藏 Actor
	UFUNCTION()
		void HideActor();

	// 呈现时间
	UPROPERTY(EditAnywhere , Category = "Show")
		float showTime;//分数属性

	// 从Z轴偏移位置呈现
	UPROPERTY(EditAnywhere , Category = "Show")
		float ZFromShow;


	// 空闲移动速度(大小以及方向)
	UPROPERTY(EditAnywhere, Category = "Idle")
		FVector IdleMoveSpeed;

	// 是否进行空闲移动
	UPROPERTY(EditAnywhere, Category = "Idle")
		bool CanIdleMove;

	// Floor的分数
	UPROPERTY(EditAnywhere, Category = "Score")
		int score;
	
	float showSpeed;//呈现速度
	float remainShowTime;//剩余的呈现时间
	float remainHideTime;//剩余的隐藏时间
	bool isIdleMove;//是否可以开始空闲移动
	float IdleRunTime;//空闲移动开始后的时间

	
};
