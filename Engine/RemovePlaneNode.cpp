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

// ***** BEGIN PYTHON BLOCK *****
// from <https://docs.python.org/3/c-api/intro.html#include-files>:
// "Since Python may define some pre-processor definitions which affect the standard headers on some systems, you must include Python.h before any standard headers are included."
#include <Python.h>
// ***** END PYTHON BLOCK *****


#include "RemovePlaneNode.h"

#include "Engine/KnobTypes.h"


static const char* removeParamName[4] = {"removePlane1", "removePlane2", "removePlane3", "removePlane4"};

#define kPlaneToRemoveParamHint "Any matching planes in input will not be available in output of this node"

#define kOperationParam "operation"
#define kOperationParamLabel "Operation"
#define kOperationParamHint "Remove: The plane(s) selected by the parameters are removed\n" \
"Keep: All but selected plane(s) are removed"

NATRON_NAMESPACE_ENTER;

struct RemovePlaneNodePrivate
{
    KnobChoiceWPtr operationKnob;
    KnobChoiceWPtr removePlaneKnob[4];

    RemovePlaneNodePrivate()
    {

    }
};

PluginPtr
RemovePlaneNode::createPlugin()
{
    std::vector<std::string> grouping;
    grouping.push_back(PLUGIN_GROUP_CHANNEL);
    PluginPtr ret = Plugin::create((void*)RemovePlaneNode::create, (void*)RemovePlaneNode::createRenderClone, PLUGINID_NATRON_REMOVE_PLANE, "RemovePlane", 1, 0, grouping);

    QString desc = tr("This node acts as a pass-through for the input image, but allows to remove existing plane(s) from the input");
    ret->setProperty<std::string>(kNatronPluginPropDescription, desc.toStdString());
    ret->setProperty<int>(kNatronPluginPropRenderSafety, (int)eRenderSafetyFullySafe);
    return ret;
}


RemovePlaneNode::RemovePlaneNode(const NodePtr& node)
: NoOpBase(node)
, _imp(new RemovePlaneNodePrivate())
{
}

RemovePlaneNode::RemovePlaneNode(const EffectInstancePtr& mainInstance, const FrameViewRenderKey& key)
: NoOpBase(mainInstance, key)
, _imp(new RemovePlaneNodePrivate())
{
}

RemovePlaneNode::~RemovePlaneNode()
{
    
}

void
RemovePlaneNode::fetchRenderCloneKnobs()
{
    _imp->operationKnob = getKnobByNameAndType<KnobChoice>(kOperationParam);
    for (int i = 0; i < 4; ++i) {
        _imp->removePlaneKnob[i] = getKnobByNameAndType<KnobChoice>(removeParamName[i]);
    }
}

std::string
RemovePlaneNode::getInputLabel(int /*inputNb*/) const
{
    return tr("Source").toStdString();
}

EffectInstance::PassThroughEnum
RemovePlaneNode::isPassThroughForNonRenderedPlanes() const
{
    return ePassThroughBlockNonRenderedPlanes;
}

void
RemovePlaneNode::initializeKnobs()
{
    KnobPagePtr mainPage = createKnob<KnobPage>("controlsPage");
    mainPage->setLabel(tr("Controls"));

    {
        KnobChoicePtr param = createKnob<KnobChoice>(kOperationParam);
        param->setLabel(tr(kOperationParamLabel));
        param->setHintToolTip(tr(kOperationParamHint));
        std::vector<ChoiceOption> options;
        options.push_back(ChoiceOption("Remove"));
        options.push_back(ChoiceOption("Keep"));
        param->populateChoices(options);
        param->setIsMetadataSlave(true);
        mainPage->addKnob(param);
        _imp->operationKnob = param;
    }
    for (int i = 0; i < 4; ++i) {
        KnobChoicePtr param = createKnob<KnobChoice>(removeParamName[i]);
        if (i == 0) {
            param->setLabel(tr("Remove Plane"));
        } else {
            param->setLabel(tr("And"));
        }
        param->setHintToolTip(tr(kPlaneToRemoveParamHint));
        param->setIsMetadataSlave(true);
        mainPage->addKnob(param);
        _imp->removePlaneKnob[i] = param;
    }
}

void
RemovePlaneNode::onMetadataChanged(const NodeMetadata& metadata)
{
    EffectInstance::onMetadataChanged(metadata);

    std::vector<ChoiceOption> choices;
    choices.push_back(ChoiceOption("None", "", ""));


    std::list<ImagePlaneDesc> availableComponents;
    {
        ActionRetCodeEnum stat = getAvailableLayers(getCurrentRenderTime(), ViewIdx(0), 0, &availableComponents);
        (void)stat;
    }

    for (std::list<ImagePlaneDesc>::const_iterator it2 = availableComponents.begin(); it2 != availableComponents.end(); ++it2) {
        ChoiceOption layerOption = it2->getPlaneOption();
        choices.push_back(layerOption);
    }

    for (int i = 0; i < 4; ++i) {
        _imp->removePlaneKnob[i].lock()->populateChoices(choices);
    }


}

ActionRetCodeEnum
RemovePlaneNode::getLayersProducedAndNeeded(TimeValue time,
                                            ViewIdx view,
                                            std::map<int, std::list<ImagePlaneDesc> >* /*inputLayersNeeded*/,
                                            std::list<ImagePlaneDesc>* layersProduced,
                                            TimeValue* passThroughTime,
                                            ViewIdx* passThroughView,
                                            int* passThroughInputNb)
{
    *passThroughInputNb = 0;
    *passThroughTime = time;
    *passThroughView = view;

    std::list<ImagePlaneDesc> availablePlanes;
    getAvailableLayers(time, view, 0, &availablePlanes);

    const bool operationIsRemove = _imp->operationKnob.lock()->getValue() == 0;

    std::list<ImagePlaneDesc> selectedPlanes, planesToKeep;
    for (int i = 0; i < 4; ++i) {
        std::string planeID = _imp->removePlaneKnob[i].lock()->getActiveEntry().id;
        if (planeID == "None") {
            continue;
        }

        for (std::list<ImagePlaneDesc>::iterator it2 = availablePlanes.begin(); it2 != availablePlanes.end(); ++it2) {
            if (planeID == it2->getPlaneID()) {
                if (operationIsRemove) {
                    availablePlanes.erase(it2);
                } else {
                    planesToKeep.push_back(*it2);
                }
                break;
            }
        }
    }
    if (!operationIsRemove) {
        *layersProduced = planesToKeep;
    } else {
        *layersProduced = availablePlanes;
    }

    return eActionStatusOK;
}

NATRON_NAMESPACE_EXIT;
