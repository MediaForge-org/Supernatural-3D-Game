#pragma once
// HAND-WRITTEN STAND-IN for the few Unreal Engine declarations that the M2 movement code touches.
// It is NOT the engine: every signature here is what the author BELIEVES the engine has. A clean compile against it proves only that
// header and source files of this project agree with each other and with that belief. It cannot prove that the engine really has
// these members (see Docs/Setup/Windows-M2A-Handover.md, "Annahmen ueber die Engine-API").
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdarg>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using int32 = std::int32_t;
using uint8 = std::uint8_t;
using TCHAR = char;
#define TEXT(x) x
#define UE_ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))
#define FORCEINLINE inline
#define WITH_EDITOR 1
#define SPNCORE_API
#define GENERATED_BODY()
#define UCLASS(...)
#define USTRUCT(...)
#define UENUM(...)
#define UPROPERTY(...)
#define UFUNCTION(...)
#define PRAGMA_DISABLE_DEPRECATION_WARNINGS

template <typename T> struct TNumericLimits : std::numeric_limits<T>
{
	static constexpr T Max() { return std::numeric_limits<T>::max(); }
	static constexpr T Lowest() { return std::numeric_limits<T>::lowest(); }
};
namespace FMath
{
	template <typename T> T Clamp(T v, T lo, T hi) { return v < lo ? lo : (v > hi ? hi : v); }
	// Mixed float/double arguments are accepted: the M1 code (LastDoorX = FMath::Max(float, FVector::X)) compiled with UE 5.8 on Windows.
	template <typename T, typename U> auto Max(T a, U b) { return a > b ? a : b; }
	template <typename T, typename U> auto Min(T a, U b) { return a < b ? a : b; }
	template <typename T> T Abs(T a) { return a < 0 ? -a : a; }
}

class FString
{
public:
	FString() = default;
	FString(const char* s) : Value(s) {}
	const char* operator*() const { return Value.c_str(); }
	bool IsEmpty() const { return Value.empty(); }
	void Reset() { Value.clear(); }
	FString& operator=(const FString&) = default;
	template <typename... Args> static FString Printf(const char* format, Args... args)
	{
		char buffer[512];
		std::snprintf(buffer, sizeof(buffer), format, args...);
		return FString(buffer);
	}
	std::string Value;
};
#define UTF8_TO_TCHAR(x) (x)

struct FName
{
	const char* Text = "";
	FName() = default;
	explicit FName(const char* t) : Text(t) {}
	bool operator==(const FName& o) const { return std::string(Text) == o.Text; }
};
#define GET_MEMBER_NAME_CHECKED(Class, Member) (FName(#Member))

struct FVector2D
{
	double X = 0.0, Y = 0.0;
	FVector2D() = default;
	FVector2D(double x, double y) : X(x), Y(y) {}
	static const FVector2D ZeroVector;
};
inline const FVector2D FVector2D::ZeroVector{};

struct FVector
{
	double X = 0.0, Y = 0.0, Z = 0.0;
	FVector() = default;
	FVector(double x, double y, double z) : X(x), Y(y), Z(z) {}
	double Size2D() const { return std::sqrt(X * X + Y * Y); }
	bool IsNearlyZero() const { return std::fabs(X) < 1e-4 && std::fabs(Y) < 1e-4 && std::fabs(Z) < 1e-4; }
	FVector GetSafeNormal2D() const { const double s = Size2D(); return s > 1e-8 ? FVector(X / s, Y / s, 0.0) : FVector(); }
	FString ToString() const { return FString::Printf("X=%f Y=%f Z=%f", X, Y, Z); }
	static double DistSquared2D(const FVector& a, const FVector& b) { return (a.X - b.X) * (a.X - b.X) + (a.Y - b.Y) * (a.Y - b.Y); }
	static const FVector ZeroVector;
};
inline const FVector FVector::ZeroVector{};
struct FBox { bool IsInside(const FVector&) const { return true; } };
struct FRotator
{
	float Pitch = 0.f, Yaw = 0.f, Roll = 0.f;
	FRotator() = default;
	FRotator(float p, float y, float r) : Pitch(p), Yaw(y), Roll(r) {}
};
struct FQuat { FVector UnrotateVector(const FVector& v) const { return v; } };
struct FTransform {};
struct FBoxSphereBounds { FBox GetBox() const { return {}; } };

class UObject { public: virtual ~UObject() = default; };
inline FString GetNameSafe(const UObject*) { return FString("object"); }
template <typename T, typename U> T* Cast(U* object) { return dynamic_cast<T*>(object); }
template <typename T, typename U> const T* Cast(const U* object) { return dynamic_cast<const T*>(object); }

template <typename T> struct TSharedPtr : std::shared_ptr<T>
{
	using std::shared_ptr<T>::shared_ptr;
	TSharedPtr(std::shared_ptr<T> other) : std::shared_ptr<T>(std::move(other)) {}
	T* Get() const { return this->get(); }
};
template <typename T> using TSharedRef = TSharedPtr<T>;
template <typename T, typename... Args> TSharedPtr<T> MakeShared(Args&&... args) { return TSharedPtr<T>(std::make_shared<T>(std::forward<Args>(args)...)); }
template <typename T> struct TArray : std::vector<T>
{
	int32 Num() const { return static_cast<int32>(this->size()); }
};

struct FPropertyChangedEvent { FName GetMemberPropertyName() const { return FName(); } };

class UWorld { public: double GetTimeSeconds() const { return 0.0; } };
class AActor;
class ACharacter;

class USceneComponent : public UObject
{
public:
	FQuat GetComponentQuat() const { return {}; }
};
class UPrimitiveComponent : public USceneComponent {};
class UActorComponent : public UObject
{
public:
	virtual void InitializeComponent() {}
	virtual void BeginPlay() {}
	virtual void PostEditChangeProperty(FPropertyChangedEvent&) {}
	AActor* GetOwner() const { return nullptr; }
	UWorld* GetWorld() const { return nullptr; }
};

enum EMovementMode { MOVE_None, MOVE_Walking, MOVE_NavWalking, MOVE_Falling, MOVE_Swimming, MOVE_Flying, MOVE_Custom };

class FNetworkPredictionData_Client { public: virtual ~FNetworkPredictionData_Client() = default; };
class UCharacterMovementComponent;
class FSavedMove_Character;
using FSavedMovePtr = TSharedPtr<FSavedMove_Character>;
class FNetworkPredictionData_Client_Character : public FNetworkPredictionData_Client
{
public:
	explicit FNetworkPredictionData_Client_Character(const UCharacterMovementComponent&) {}
	virtual FSavedMovePtr AllocateNewMove();
};
class FSavedMove_Character
{
public:
	enum CompressedFlags { FLAG_JumpPressed = 0x01, FLAG_WantsToCrouch = 0x02, FLAG_Reserved_1 = 0x04, FLAG_Reserved_2 = 0x08,
		FLAG_Custom_0 = 0x10, FLAG_Custom_1 = 0x20, FLAG_Custom_2 = 0x40, FLAG_Custom_3 = 0x80 };
	virtual ~FSavedMove_Character() = default;
	virtual void Clear() {}
	virtual uint8 GetCompressedFlags() const { return 0; }
	virtual bool CanCombineWith(const FSavedMovePtr&, ACharacter*, float) const { return true; }
	virtual void SetMoveFor(ACharacter*, float, FVector const&, FNetworkPredictionData_Client_Character&) {}
	virtual void PrepMoveFor(ACharacter*) {}
};
inline FSavedMovePtr FNetworkPredictionData_Client_Character::AllocateNewMove() { return MakeShared<FSavedMove_Character>(); }

class UCharacterMovementComponent : public UActorComponent
{
public:
	USceneComponent* UpdatedComponent = nullptr;
	FVector Velocity, Acceleration;
	EMovementMode MovementMode = MOVE_Walking;
	float MaxWalkSpeed = 600.f, MaxAcceleration = 2048.f, BrakingDecelerationWalking = 2048.f, BrakingDecelerationFalling = 0.f;
	float MaxWalkSpeedCrouched = 300.f, BrakingFriction = 0.f, GroundFriction = 8.f, MaxStepHeight = 45.f, AirControl = 0.05f;
	float JumpZVelocity = 420.f, MinAnalogWalkSpeed = 0.f;
	bool bUseSeparateBrakingFriction = false, bOrientRotationToMovement = false;
	FRotator RotationRate;
	class FNetworkPredictionData_Client_Character* ClientPredictionData = nullptr;
	virtual float GetMaxSpeed() const { return MaxWalkSpeed; }
	virtual float GetMaxAcceleration() const { return MaxAcceleration; }
	virtual float GetGravityZ() const { return -980.f; }
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const { return nullptr; }
	virtual void UpdateFromCompressedFlags(uint8) {}
	virtual bool ClientUpdatePositionAfterServerUpdate() { return false; }
	virtual void OnMovementModeChanged(EMovementMode, uint8) {}
	bool IsCrouching() const { return false; }
	bool IsFalling() const { return MovementMode == MOVE_Falling; }
	bool IsMovingOnGround() const { return MovementMode == MOVE_Walking || MovementMode == MOVE_NavWalking; }
	void StopMovementImmediately() {}
};

class APawn;
class AActor : public UObject
{
public:
	struct { bool bCanEverTick = true; } PrimaryActorTick;
	virtual void OnConstruction(const FTransform&) {}
	virtual void PostInitializeComponents() {}
	bool HasAuthority() const { return true; }
	FVector GetActorLocation() const { return {}; }
	FRotator GetActorRotation() const { return {}; }
	bool TeleportTo(const FVector&, const FRotator&, bool = false, bool = false) { return true; }
	void SetRootComponent(USceneComponent*) {}
	template <typename T> T* CreateDefaultSubobject(const char*) { return new T(); }
};
class APawn : public AActor {};
class ACharacter : public APawn
{
public:
	UCharacterMovementComponent* GetCharacterMovement() const { return nullptr; }
};

#define LogSupernaturalProt 0
#define UE_LOG(Category, Verbosity, Format, ...) do { (void)(Category); (void)sizeof(FString::Printf(Format, ##__VA_ARGS__)); } while (0)
