// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"



// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creates Spring Arm. Pulls towards player if collision
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	//CameraArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	CameraArm->TargetArmLength = 350.f;//Camera follow distance
	CameraArm->bUsePawnControlRotation = true;//Rotate arm based on controller

	//Creates Camera that follow Character
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	//camera will rotate based on the spring arm which rotates based on the character
	FollowCamera->bUsePawnControlRotation = false;

	//Turn Rate for input
	BaseTurnRate = 65.f;
	BaseLookRate = 65.f;

	//Keep character from rotating with camera
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false; 
	
	//Confirgures Character Movement
	GetCharacterMovement()->bOrientRotationToMovement = true; //Move character in the direction of input
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);//Range of direction for character
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.5f;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("ForwardMove", this, &AMainCharacter::ForwardMove);
	PlayerInputComponent->BindAxis("HorizontalMove", this, &AMainCharacter::HorizontalMove);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMainCharacter::TurnRate);
	PlayerInputComponent->BindAxis("LookRate", this, &AMainCharacter::LookRate);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this , &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}
void AMainCharacter::ForwardMove(float value) {
	if (Controller != nullptr && value != 0.0f) {
		//Allows player to pick forward move
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		//Gets forward Vector based off rotation
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}
void AMainCharacter::HorizontalMove(float value) {
	if (Controller != nullptr && value != 0.0f) {
		//Allows player to pick horizontal move
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		//Gets horizontal Vector based off rotation
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
}
void AMainCharacter::TurnRate(float value) {
	AddControllerYawInput(value * BaseTurnRate * GetWorld()-> GetDeltaSeconds());
}
void AMainCharacter::LookRate(float value) {
	AddControllerPitchInput(value * BaseLookRate * GetWorld()->GetDeltaSeconds());
}