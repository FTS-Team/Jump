// Fill out your copyright notice in the Description page of Project Settings.

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


	// ����ǰ���ͺ��˵����롣
	UFUNCTION()
		void MoveForward(float Value);

	// ���������ƶ������롣
	UFUNCTION()
		void MoveRight(float Value);


	// ���°���ʱ---��ʼ����
	UFUNCTION(BlueprintCallable, Category = "RedCharacter")
		void ReadyJump();

	// ȡ������
	UFUNCTION(BlueprintCallable, Category = "RedCharacter")
		void cancelJump();

	// ��ͣ����
	UFUNCTION(BlueprintCallable, Category = "RedCharacter")
		void pauseJump();

	// ��������
	UFUNCTION(BlueprintCallable, Category = "RedCharacter")
		void ResumeJump();

	// �ɿ�����ʱ---��������
	UFUNCTION(BlueprintCallable, Category = "RedCharacter")
		void StartJump();
	

	// ��ʼ׼����Ծ�¼�����
	UFUNCTION(BlueprintImplementableEvent)
		void ReadyJumpDrive();

	// ȡ��׼����Ծ�¼�����
	UFUNCTION(BlueprintImplementableEvent)
		void CancelJumpDrive();

	// ��ͣ�����¼�����
	UFUNCTION(BlueprintImplementableEvent)
		void PauseJumpDrive();

	// ���������¼�����
	UFUNCTION(BlueprintImplementableEvent)
		void ResumeJumpDrive();

	// ��ʼ��Ծ�¼�����
	UFUNCTION(BlueprintImplementableEvent)
		void StartJumpDrive();

	// ������Ծ�¼�����
	UFUNCTION(BlueprintImplementableEvent)
		void EndJumpDrive();


	// ��ӷ����¼�����
	UFUNCTION(BlueprintImplementableEvent)
		void AddScoreDrive();

	// ��ʾ�������¼�����
	UFUNCTION(BlueprintImplementableEvent)
		void PowerDrive();

	// �����������¼�
	UFUNCTION(BlueprintImplementableEvent)
		void FullPowerDrive();

	// ������Ϸ�¼�����
	UFUNCTION(BlueprintImplementableEvent)
		void GameOverDrive();

	// ��ͣ��Ϸ
	UFUNCTION()
		void setPause();

	// ���¿�ʼ
	UFUNCTION()
		void reStartLevel();

	// ���������ٶ�
	UFUNCTION()
		void setJumpSpeed(float speed);

	// �ı䷽��
	UFUNCTION()
		void changeDirection();

	// ��ȡ��һ������
	UFUNCTION()
		int getNextFloor();

	// ����ǰfloor���ӷ���
	UFUNCTION()
	void addScore();


	// ���õ�ǰ��Ծ����ʱ��
	UFUNCTION()
		void setJumpTime(float time);


	UPROPERTY(EditAnywhere)
		class USpringArmComponent * SpringArm;//���ɱ�

	const int speed = 300;//ˮƽ�����ٶȣ�speed * jumpTime = ����ˮƽ�ٶ�)
	const float minVertiSpeed = 400;//��С��ֱ�ٶ�
	const float timeOfChange = 1;//�ı䷽������ʱ��
	const float maxJumpTime = 5;//�������ʱ��
	
	bool onReadyJump;//������Ծ��״̬
	bool canChangeDirection;//���Կ�ʼ�ı䷽���״̬
	bool onPauseJump;//��ͣ������Ծ��״̬
	bool onFullPower;//������״̬

	float jumpTime;//����ʱ��
	float remainTimeOfChange;//�ı䷽���ʣ��ʱ��
	FRotator velocityOfChange;//�ı䷽����ٶ�

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int score;//��������

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float power;//��������

	
	//����ľ��
	int cur_floor;
	TArray<AActor *> floorsArray;

};
