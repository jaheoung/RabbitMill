// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetBase.h"

bool UWidgetBase::Opened()
{
	return isOpened;
}

void UWidgetBase::Open()
{
	isOpened = true;
}

void UWidgetBase::Close()
{
	isOpened = false;
}