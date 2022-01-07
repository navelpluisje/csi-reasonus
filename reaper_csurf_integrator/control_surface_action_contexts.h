//
//  control_surface_base_actions.h
//  reaper_csurf_integrator
//
//

#ifndef control_surface_action_contexts_h
#define control_surface_action_contexts_h

#include "control_surface_integrator.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NoAction : public Action
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
public:
    virtual string GetName() override { return "NoAction"; }
    
    virtual void RequestUpdate(ActionContextOld* context) override
    {
        context->ClearWidget();
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ReaperAction : public Action
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
public:
    virtual string GetName() override { return "ReaperAction"; }
    
    virtual double GetCurrentNormalizedValue(ActionContextOld* context) override
    {
        return DAW::GetToggleCommandState(context->GetCommandId());
    }

    virtual void RequestUpdate(ActionContextOld* context) override
    {
        context->UpdateWidgetValue(GetCurrentNormalizedValue(context));
    }
    
    virtual void Do(ActionContextOld* context, double value) override
    {
        if(value != 0)
            DAW::SendCommandMessage(context->GetCommandId());
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FXAction : public Action
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
public:
    virtual string GetName() override { return "FXAction"; }
    
    virtual double GetCurrentNormalizedValue(ActionContextOld* context) override
    {
        if(MediaTrack* track = context->GetTrack())
        {
            double min, max = 0.0;
            
            return DAW::TrackFX_GetParam(track, context->GetSlotIndex(), context->GetParamIndex(), &min, &max);
        }
        else
            return 0.0;
    }

    virtual void RequestUpdate(ActionContextOld* context) override
    {
        if(MediaTrack* track = context->GetTrack())
        {
            double currentValue = GetCurrentNormalizedValue(context);
            
            if(context->GetShouldUseDisplayStyle())
                context->UpdateWidgetValue(context->GetDisplayStyle(), currentValue);
            else
                context->UpdateWidgetValue(currentValue);
        }
        else
            context->ClearWidget();
    }
};

#endif /* control_surface_action_contexts_h */
