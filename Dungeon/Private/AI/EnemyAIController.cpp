// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"//
#include "BehaviorTree/BehaviorTreeComponent.h"//

AEnemyAIController::AEnemyAIController()
{

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackBoard");
	check(Blackboard);
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTree");
	check(BehaviorTreeComponent);
}