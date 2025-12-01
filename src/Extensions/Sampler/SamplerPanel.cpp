/*
 *
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "SamplerPanel.h"
#include "Sample.h"
#include "SamplerTrackBoost.h"
#include "GlobalDef.h"
#include "GfxMsg.h"
#include "SamplerTrack.h"
#include "Xed_Utils.h"
#include "sampler_locale.h"

#include "XDigit.h"
#include "XHost.h"

#include <Autolock.h>

#define	REMOVE		'remv'
#define	REMOVEALL	'rema'
#define	LOADEXT		'loae'
#define REL_MSG 	'note'
#define REV_ON		'reo '
#define	PIT_ON 		'pio '
#define	BOOST_ON 	'boo '
#define	MOD			'mod '
#define LOOP_ON		'loop'



SamplerPanel::SamplerPanel(SamplerTrackBoost* sb):
			  PlugPanel(), sampTrack(NULL), booster(sb)
{
	// Create menu
	menu = new BMenu(" ");
	menu->AddItem(booster->getMenu());
	menu->AddItem(new BMenuItem(T_SAMPLER_LOAD, new BMessage(LOADEXT)));
	menu->AddItem(new BMenuItem(T_SAMPLER_REMOVE, new BMessage(REMOVE)));
	menu->AddItem(new BMenuItem(T_SAMPLER_REMOVE_ALL, new BMessage(REMOVEALL)));

	// Create menu field
	field = new BMenuField("", " ", menu);
	field->SetDivider(0);

	// Create sample selector box
	BBox* sampler = new BBox("toolbar");
	BLayoutBuilder::Group<>(sampler, B_HORIZONTAL)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(field);

	// Create pitch/stretch controls
	pit_ck = new BCheckBox("", T_SAMPLER_STRECH, new BMessage(PIT_ON));
	pit_ck->SetValue(0);
	shift = new XDigit(BRect(0, 0, 36, 21), VID_EMPTY, "sampler_shift_xdigit", new BMessage(MOD), 1, 32);

	BBox* pitch_box = new BBox("");
	BLayoutBuilder::Group<>(pitch_box, B_HORIZONTAL)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(pit_ck)
		.AddGlue()
		.Add(shift);

	// Create boost controls
	boost_ck = new BCheckBox("", T_SAMPLER_BOOST, new BMessage(BOOST_ON));
	boost_ck->SetValue(0);
	depth = new XDigit(BRect(0, 0, 36, 21), VID_EMPTY, "sampler_boost", new BMessage(REL_MSG), 1, 4);

	BBox* boost_box = new BBox("");
	BLayoutBuilder::Group<>(boost_box, B_HORIZONTAL)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(boost_ck)
		.AddGlue()
		.Add(depth);

	// Create reverse and loop controls
	rev = new BCheckBox("rev_check", T_SAMPLER_REVERSE, new BMessage(TRACK_REV));
	rev->SetValue(0);

	loop_ck = new BCheckBox("loop_check", "Loop", new BMessage(LOOP_ON));
	loop_ck->SetValue(0);

	BBox* options_box = new BBox("");
	BLayoutBuilder::Group<>(options_box, B_HORIZONTAL)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(rev)
		.Add(loop_ck)
		.AddGlue();

	// Create sample view
	sw = new SampleView(XUtils::GetBitmap(18));
	sw->SetExplicitMinSize(BSize(159, 58));
	sw->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, 58));

	BBox* sam_box = new BBox("");
	BLayoutBuilder::Group<>(sam_box, B_VERTICAL, 0)
		.SetInsets(1, 1, 1, 1)
		.Add(sw);

	// Build main layout
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(sampler)
		.Add(pitch_box)
		.Add(boost_box)
		.Add(options_box)
		.Add(sam_box);

	my_sample = NULL;
	rev->SetValue(false);
	menu->Superitem()->SetLabel(T_SAMPLER_NOSELECTED);
}

void
SamplerPanel::ResetToTrack(Track* trk)
{
	//qui magari un bel check dell'ID ??
	PlugPanel::ResetToTrack(trk);
	SetTrack((SamplerTrack*)trk);
}

void
SamplerPanel::AttachedToWindow()
{
	PlugPanel::AttachedToWindow();
	depth->SetTarget(this);
	shift->SetTarget((BView*)this);
	pit_ck->SetTarget(this);
	rev->SetTarget(this);
	menu->SetTargetForItems(this);
	boost_ck->SetTarget(this);
	loop_ck->SetTarget(this);
}
void
SamplerPanel::SetTrack(SamplerTrack *tr)
{
	if(!Window()) return;

	sampTrack = tr;

	// RAII: BAutolock automatically unlocks on scope exit (including early returns/exceptions)
	BAutolock lock(Window());
	if(!lock.IsLocked()) return;

	if(tr == NULL || tr->getSample() == NULL)
	{

		sw->Init(NULL, false, false);
		shift->UpdateValue(16, true);
		pit_ck->SetValue(false);
		boost_ck->SetValue(false);
		loop_ck->SetValue(false);
		menu->Superitem()->SetLabel(T_SAMPLER_NOSELECTED);
		depth->UpdateValue(1, true);
	}
	else
	{
		SetTitle(tr->getName());
		my_sample=tr->getSample();
		sw->Init(my_sample, tr->isReversed(), 1.0f);

		menu->Superitem()->SetLabel(my_sample->GetName());
		shift->UpdateValue(tr->getResample(), true);
		pit_ck->SetValue(tr->isResampleEnable());
		boost_ck->SetValue(tr->isBoostEnable());
		loop_ck->SetValue(tr->IsLoopEnable());
		rev->SetValue(tr->isReversed());
		depth->UpdateValue((int32)tr->amp, true);
		sw->SetBoost(tr->amp);
	}
}

void
SamplerPanel::MessageReceived(BMessage* message)
{

	switch(message->what)
	{
		case LOOP_ON:
			if(sampTrack == NULL) return;
			sampTrack->SetLoopEnable((bool)loop_ck->Value());
		break;
		case BOOST_ON:
			if(sampTrack==NULL) return;
			sampTrack->setBoostEnable((bool)boost_ck->Value());
			if(!boost_ck->Value())
			{
				sampTrack->amp=1.0;
				sw->SetBoost(sampTrack->amp);
				return;
			}
			//else continue (without break!)
		case REL_MSG:
			if(sampTrack==NULL) return;

			if(!sampTrack->isBoostEnable()) return;

			sampTrack->amp=(float)depth->GetValue();
			sw->SetBoost(sampTrack->amp);

		break;

	 	case TRACK_SAMP_EXT:
	 		booster->ChangeSample(message->FindInt16("sample"));//ok
	 	break;
	 case MOD:
	 		if(sampTrack==NULL) return;
	 		{
	 			// RAII: XHostLock automatically unlocks on scope exit
	 			XHostLock lock("SamplerPanel::MOD");
	 			sampTrack->setResample(shift->GetValue());
	 		}
	 	break;
	 case PIT_ON:
	 		if(sampTrack==NULL) return;
	 		{
	 			// RAII: XHostLock automatically unlocks on scope exit
	 			XHostLock lock("SamplerPanel::PIT_ON");
	 			sampTrack->setResampleEnable((bool)pit_ck->Value());
	 		}
	 	break;
	 	case TRACK_REV:
			if(sampTrack==NULL) return;
			sampTrack->setReversed(rev->Value());
			sw->SetReversed(rev->Value());

		break;
		case LOADEXT:
			booster->LoadSample();
		break;
		case REMOVEALL:
			booster->RemoveAll();
		break;
		case REMOVE:
			booster->RemoveSample(((SamplerTrack*)sampTrack)->getSample());
		break;
		case B_REFS_RECEIVED:	//ok
		{

			entry_ref	ref;
			if(message->FindRef("refs",&ref)==B_OK)
			{
					booster->RefReceived(ref,sampTrack);
					booster->RefreshSelected();
			}
		}
		break;
	 	default:
			PlugPanel::MessageReceived(message);
		break;
	}
}
