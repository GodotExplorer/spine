/******************************************************************************
 * Spine Runtimes Software License v2.5
 *
 * Copyright (c) 2013-2016, Esoteric Software
 * All rights reserved.
 *
 * You are granted a perpetual, non-exclusive, non-sublicensable, and
 * non-transferable license to use, install, execute, and perform the Spine
 * Runtimes software and derivative works solely for personal or internal
 * use. Without the written permission of Esoteric Software (see Section 2 of
 * the Spine Software License Agreement), you may not (a) modify, translate,
 * adapt, or develop new applications using the Spine Runtimes or otherwise
 * create derivative works or improvements of the Spine Runtimes or (b) remove,
 * delete, alter, or obscure any trademarks or any copyright, trademark, patent,
 * or other intellectual property or proprietary rights notices on or in the
 * Software, including any copy thereof. Redistributions in binary or source
 * form must include this license and terms.
 *
 * THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL ESOTERIC SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES, BUSINESS INTERRUPTION, OR LOSS OF
 * USE, DATA, OR PROFITS) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/
#ifdef MODULE_SPINE_ENABLED
#ifndef SPINE_H
#define SPINE_H

#include "scene/2d/node_2d.h"
#include "scene/resources/shape_2d.h"
#include <spine/spine.h>
#include "spine_batcher.h"
#include "core/array.h"

class CollisionObject2D;

class Spine : public Node2D {

	GDCLASS(Spine, Node2D);
public:
	enum AnimationProcessMode {
		ANIMATION_PROCESS_FIXED,
		ANIMATION_PROCESS_IDLE,
	};
	enum DebugAttachmentMode {
		DEBUG_ATTACHMENT_REGION,
		DEBUG_ATTACHMENT_MESH,
		DEBUG_ATTACHMENT_SKINNED_MESH,
		DEBUG_ATTACHMENT_BOUNDING_BOX,
	};

	class SpineResource : public Resource {

		GDCLASS(SpineResource, Resource);


	public:

		SpineResource();
		~SpineResource();

		spAtlas *atlas;
		spSkeletonData *data;
	};

private:
	Ref<SpineResource> res;

	spSkeleton* skeleton;
	spBone* root_bone;
	spAnimationState* state;
	mutable Vector<float> world_verts;

	float speed_scale;
	String autoplay;
	AnimationProcessMode animation_process_mode;
	bool processing;
	bool active;
	bool playing;
	bool forward;
	int skip_frames;
	int frames_to_skip;
	float process_delta;
	bool debug_bones;
	bool debug_attachment_region;
	bool debug_attachment_mesh;
	bool debug_attachment_skinned_mesh;
	bool debug_attachment_bounding_box;
	String current_animation;
	float duration; // Handled as a property, but never set in the setter
	float actual_duration; // Store the actual length of the animation
	bool loop;
	String skin;

	Color modulate;
	bool flip_x, flip_y;
	SpineBatcher batcher;

	// fx slots (always show on top)
	Node2D *fx_node;
	SpineBatcher fx_batcher;
	CharString fx_slot_prefix;

	float current_pos;

	typedef struct AttachmentNode {
		List<AttachmentNode>::Element *E;
		spBone *bone;
		WeakRef *ref;
		Vector2 ofs;
		Vector2 scale;
		real_t rot;
	} AttachmentNode;
	typedef List<AttachmentNode> AttachmentNodes;
	AttachmentNodes attachment_nodes;

	static void spine_animation_callback(spAnimationState* p_state, spEventType p_type, spTrackEntry* p_track, spEvent* p_event);
	void _on_animation_state_event(int p_track, spEventType p_type, spEvent *p_event, int p_loop_count);

	void _spine_dispose();
	void _animation_process(float p_delta);
	void _animation_draw();
	void _set_process(bool p_process, bool p_force = false);
	void _on_fx_draw();
	void _update_verties_count();

protected:
	static Array *invalid_names;

	bool _set(const StringName& p_name, const Variant& p_value);
	bool _get(const StringName& p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	void _notification(int p_what);

	static void _bind_methods();

public:
	static Array get_invalid_names();

	// set/get spine resource
	void set_resource(Ref<SpineResource> p_data);
	Ref<SpineResource> get_resource();

	Array get_animation_names() const;

	bool has_animation(const String& p_name);
	void set_default_mix(real_t p_duration);
	void mix(const String& p_from, const String& p_to, real_t p_duration);

	bool play(const String& p_name, bool p_loop = false, int p_track = 0, int p_delay = 0);
	bool add(const String& p_name, bool p_loop = false, int p_track = 0, int p_delay = 0);
	void clear(int p_track = -1);
	void stop();
	bool is_playing(int p_track = 0) const;
	float get_animation_length(String p_animation) const;
	void set_forward(bool p_forward = true);
	bool is_forward() const;
	void set_skip_frames(int p_skip_frames);
	int get_skip_frames() const;
	String get_current_animation(int p_track);
	void stop_all();
	void reset();
	void seek(float p_pos);
	float tell() const;

	void set_active(bool p_active);
	bool is_active() const;

	void set_speed(float p_speed);
	float get_speed() const;

	void set_autoplay(const String& p_name);
	String get_autoplay() const;

	void set_modulate(const Color& p_color);
	Color get_modulate() const;

	void set_flip_x(bool p_flip);
	void set_flip_y(bool p_flip);
	bool is_flip_x() const;
	bool is_flip_y() const;

	void set_duration(float p_duration);
	float get_duration() const;

	void set_animation_process_mode(AnimationProcessMode p_mode);
	AnimationProcessMode get_animation_process_mode() const;

	/* Sets the skin used to look up attachments not found in the SkeletonData defaultSkin. Attachments from the new skin are
	* attached if the corresponding attachment from the old skin was attached. If there was no old skin, each slot's setup mode
	* attachment is attached from the new skin. Returns false if the skin was not found.
	* @param skin May be 0.*/
	bool set_skin(const String& p_name);

	//spAttachment* get_attachment(const char* slotName, const char* attachmentName) const;
	Dictionary get_skeleton() const;
	/* Returns null if the slot or attachment was not found. */
	Dictionary get_attachment(const String& p_slot_name, const String& p_attachment_name) const;
	/* Returns null if the bone was not found. */
	Dictionary get_bone(const String& p_bone_name) const;
	/* Returns null if the slot was not found. */
	Dictionary get_slot(const String& p_slot_name) const;
	/* Returns false if the slot or attachment was not found. */
	bool set_attachment(const String& p_slot_name, const Variant& p_attachment);
	// bind node to bone, auto update pos/rotate/scale
	bool has_attachment_node(const String& p_bone_name, const Variant& p_node);
	bool add_attachment_node(const String& p_bone_name, const Variant& p_node, const Vector2& p_ofs = Vector2(0, 0), const Vector2& p_scale = Vector2(1, 1), const real_t p_rot = 0);
	bool remove_attachment_node(const String& p_bone_name, const Variant& p_node);
	// get spine bounding box
	Ref<Shape2D> get_bounding_box(const String& p_slot_name, const String& p_attachment_name);
	// bind collision object 2d to spine bounding box
	bool add_bounding_box(const String& p_bone_name, const String& p_slot_name, const String& p_attachment_name, const Variant& p_node, const Vector2& p_ofs = Vector2(0, 0), const Vector2& p_scale = Vector2(1, 1), const real_t p_rot = 0);
	bool remove_bounding_box(const String& p_bone_name, const Variant& p_node);

	void set_fx_slot_prefix(const String& p_prefix);
	String get_fx_slot_prefix() const;

	void set_debug_bones(bool p_enable);
	bool is_debug_bones() const;
	void set_debug_attachment(DebugAttachmentMode p_mode, bool p_enable);
	bool is_debug_attachment(DebugAttachmentMode p_mode) const;

	//void seek(float p_time, bool p_update = false);
	//void seek_delta(float p_time, float p_delta);
	//float get_current_animation_pos() const;
	//float get_current_animation_length() const;

	//void advance(float p_time);

	virtual Rect2 _edit_get_rect() const;

	Spine();
	virtual ~Spine();
};

VARIANT_ENUM_CAST(Spine::AnimationProcessMode);
VARIANT_ENUM_CAST(Spine::DebugAttachmentMode);

#endif // SPINE_H
#endif // MODULE_SPINE_ENABLED

