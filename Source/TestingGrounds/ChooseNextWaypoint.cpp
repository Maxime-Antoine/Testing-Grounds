// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get patrol points
	const auto BlackBoardComp = OwnerComp.GetBlackboardComponent();
	const auto Index = BlackBoardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	const auto Waypoint = BlackBoardComp->GetValueAsObject(WaypointKey.SelectedKeyName);
	const auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	const auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();
	if (!ensure(PatrolRoute)) return EBTNodeResult::Failed;

	const auto PatrolPoints = PatrolRoute->GetPatrolPoints();
	if (PatrolPoints.Num() == 0) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing patrol points"))
			return EBTNodeResult::Failed;
	}

	// Set next waypoint
	BlackBoardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
	const auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackBoardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}