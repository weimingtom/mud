//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <edit/Generated/Forward.h>
#include <edit/Tool.h>

#include <vector>

namespace mud
{
	class _refl_ WorkPlaneAction : public EditorAction
	{
	public:
		WorkPlaneAction(Plane& workPlane, const Plane& plane);

		virtual void apply() final;
		virtual void undo() final;

	private:
		Plane& m_workPlane;
		Plane m_startPlane;
		Plane m_endPlane;
	};

	class _refl_ WorkPlaneTool : public Tool
	{
	public:
		WorkPlaneTool(ToolContext& context, cstring name, const Plane& plane);

		virtual void activate() final;

	private:
		object_ptr<WorkPlaneAction> m_action;
		Plane m_plane;
	};

	inline unique_ptr<WorkPlaneTool> xy_work_plane_tool(ToolContext& context) { return make_unique<WorkPlaneTool>(context, "XY Work Plane", Plane{ Zero3, X3, Y3 }); }
	inline unique_ptr<WorkPlaneTool> yz_work_plane_tool(ToolContext& context) { return make_unique<WorkPlaneTool>(context, "YZ Work Plane", Plane{ Zero3, Y3, Z3 }); }
	inline unique_ptr<WorkPlaneTool> xz_work_plane_tool(ToolContext& context) { return make_unique<WorkPlaneTool>(context, "XZ Work Plane", Plane{ Zero3, X3, Z3 }); }
}
