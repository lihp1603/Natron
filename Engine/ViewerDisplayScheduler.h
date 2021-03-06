/* ***** BEGIN LICENSE BLOCK *****
 * This file is part of Natron <http://www.natron.fr/>,
 * Copyright (C) 2016 INRIA and Alexandre Gauthier-Foichat
 *
 * Natron is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Natron is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Natron.  If not, see <http://www.gnu.org/licenses/gpl-2.0.html>
 * ***** END LICENSE BLOCK ***** */

#ifndef NATRON_ENGINE_VIEWERDISPLAYSCHEDULER_H
#define NATRON_ENGINE_VIEWERDISPLAYSCHEDULER_H

// ***** BEGIN PYTHON BLOCK *****
// from <https://docs.python.org/3/c-api/intro.html#include-files>:
// "Since Python may define some pre-processor definitions which affect the standard headers on some systems, you must include Python.h before any standard headers are included."
#include <Python.h>
// ***** END PYTHON BLOCK *****

#include "Engine/OutputSchedulerThread.h"

NATRON_NAMESPACE_ENTER;


/**
 * @brief An OutputSchedulerThread implementation that also update the viewer with the rendered image
 **/
class ViewerInstance;
class ViewerDisplayScheduler
: public OutputSchedulerThread
{

public:

    ViewerDisplayScheduler(RenderEngine* engine,
                           const NodePtr& viewer);

    virtual ~ViewerDisplayScheduler();

    virtual SchedulingPolicyEnum getSchedulingPolicy() const OVERRIDE FINAL { return eSchedulingPolicyOrdered; }

    virtual void processFrame(const ProcessFrameArgsBase& args) OVERRIDE FINAL;

private:


    virtual void timelineGoTo(TimeValue time) OVERRIDE FINAL;
    virtual TimeValue timelineGetTime() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual bool isFPSRegulationNeeded() const OVERRIDE FINAL WARN_UNUSED_RETURN { return true; }

    virtual void getFrameRangeToRender(TimeValue& first, TimeValue& last) const OVERRIDE FINAL;


    virtual RenderThreadTask* createRunnable(TimeValue frame, bool useRenderStarts, const std::vector<ViewIdx>& viewsToRender) OVERRIDE FINAL WARN_UNUSED_RETURN;

    virtual void handleRenderFailure(ActionRetCodeEnum stat, const std::string& errorMessage) OVERRIDE FINAL;

    virtual TimeValue getLastRenderedTime() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual void onRenderStopped(bool aborted) OVERRIDE FINAL;
};



NATRON_NAMESPACE_EXIT;

#endif // VIEWERDISPLAYSCHEDULER_H
