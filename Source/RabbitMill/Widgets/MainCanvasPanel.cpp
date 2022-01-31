// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCanvasPanel.h"

void UMainCanvasPanel::TopWidget(UWidget* widget)
{
	int index = GetChildIndex(widget);

	if (index < 0)
		return;

	UPanelSlot* backSlot = Slots[index];
	Slots.RemoveAt(index);
	Slots.Add(backSlot);

	OnSlotAdded(backSlot);
	// 다시 빌드.
	InvalidateLayoutAndVolatility();
}