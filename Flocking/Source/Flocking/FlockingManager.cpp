// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockingManager.h"
#define AGENT_COUNT 20

void UFlockingManager::Init(UWorld* world, UStaticMeshComponent* mesh) {
    UE_LOG(LogTemp, Warning, TEXT("Manager initialized"));

    World = world;
    degree = 0;
    delay = 200;
    updateTargetLocation();
    float incr = (PI * 2.f) / AGENT_COUNT;
    for (int i = 0; i < AGENT_COUNT; i++) {
        if (World != nullptr) {
            FRotator rotation = FRotator();

            FVector location = FVector();
            location.X = FMath::Sin(incr * i) * 100.f + TargetLocation.X;
            location.Y = FMath::Cos(incr * i) * 100.f + TargetLocation.Y;
            location.Z = FMath::Cos(incr * i) * 100.f + TargetLocation.Z;
           
            AAgent* agent = World->SpawnActor<AAgent>(location, rotation);
            agent->Init(mesh, i);
            Agents.Add(agent);
        }
    }

    initialized = true;
}

void UFlockingManager::Flock() {
    FVector v1, v2, v3, v4;
        updateTargetLocation();
    for (int i = 0; i < AGENT_COUNT; i++) {
        AAgent* b = Agents[i];
        v1 = rule1(b);
        v2 = rule2(b);
        v3 = rule3(b);
        v4 = rule4(b);
        b->Velocity = b->Velocity + v1 + v2 + v3 + v4;
        //Boids do their own movement if we set velocity

    }

}

FVector UFlockingManager::rule1(AAgent* bj) {
    FVector pc = FVector(0);
    for (int i = 0; i < AGENT_COUNT; i++) {
        AAgent* b = Agents[i];
        if (b != bj) {
            pc = pc + b->GetActorLocation();
        }
    }
    pc = pc / (AGENT_COUNT - 1);
    pc = (pc - bj->GetActorLocation())/ 100;

    return pc;
}
FVector UFlockingManager::rule2(AAgent* bj) {
    FVector c = FVector(0);
    for (int i = 0; i < AGENT_COUNT; i++) {
        AAgent* b = Agents[i];
        if (b != bj) {
            if ((b->GetActorLocation() - bj->GetActorLocation()).Length() < 100) {
                c = c - (b->GetActorLocation() - bj->GetActorLocation());
            }
        }
    }
    return c;
}
FVector UFlockingManager::rule3(AAgent* bj) {
    FVector pv = FVector(0);
    for (int i = 0; i < AGENT_COUNT; i++) {
        AAgent* b = Agents[i];
        if (b != bj) {
            if ((b->GetActorLocation() - bj->GetActorLocation()).Length() < 300) {
                pv = pv + b->Velocity;
            }
            
        }
    }
    pv = pv / (AGENT_COUNT - 1);
    pv = (pv - bj->Velocity) / 8;

    return pv;
}
void UFlockingManager::updateTargetLocation() {
    degree = FMath::Fmod((degree + 0.01), 360);
    TargetLocation.X = FMath::Sin(degree) * 4500.f;
    TargetLocation.Z = 5000;
    TargetLocation.Y = FMath::Cos(degree) * 4500.f;
}

FVector UFlockingManager::rule4(AAgent* bj) {
    FVector pv = FVector();
    pv = (TargetLocation - bj->GetActorLocation()) / 100;
    return pv;
}