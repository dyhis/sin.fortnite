#include <sdk/sdk.h>
#include <main/actor.h>

void sdk::bone_manager::get_bone_array(uintptr_t mesh)
{
	ue->bm.data.bone_array = km_ptr->read<uintptr_t>(mesh + 0x598 + (km_ptr->read<int>(mesh + 0x5e0) * 0x10));
	ue->bm.data.component_to_world = km_ptr->read<FTransform>(mesh + offsets.componet_to_world);
}

Vector3 sdk::bone_manager::get_bone(int id)
{
	auto bone = km_ptr->read<FTransform>(ue->bm.data.bone_array + id * 0x60);
	D3DMATRIX bone_matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ue->bm.data.component_to_world.ToMatrixWithScale());
	return Vector3(bone_matrix._41, bone_matrix._42, bone_matrix._43);
}

std::unordered_map<int, Vector3> sdk::bone_manager::get_skeleton_bones(const std::vector<std::pair<int, int>>& bone_pairs)
{
	std::unordered_map<int, Vector3> bone_positions;
	bone_positions.reserve(bone_pairs.size() * 2);

	D3DMATRIX componentToWorldMatrix = ue->bm.data.component_to_world.ToMatrixWithScale();

	for (const auto& pair : bone_pairs) {
		for (int id : {pair.first, pair.second}) {
			if (bone_positions.find(id) == bone_positions.end()) {
				FTransform bone = km_ptr->read<FTransform>(ue->bm.data.bone_array + (id * 0x60));
				D3DMATRIX boneMatrix = bone.ToMatrixWithScale();
				D3DMATRIX matrix = MatrixMultiplication(boneMatrix, componentToWorldMatrix);
				bone_positions[id] = Vector3(matrix._41, matrix._42, matrix._43);
			}
		}
	}

	return bone_positions;
}

void sdk::camera_manager::get_camera()
{
	rotation.a = km_ptr->read<double>(cached->rotation_ptr);
	rotation.b = km_ptr->read<double>(cached->rotation_ptr + 0x20);
	rotation.c = km_ptr->read<double>(cached->rotation_ptr + 0x1d0);

	camera.location = km_ptr->read<Vector3>(cached->location_ptr);
	camera.rotation.Pitch = asin(rotation.c) * (180.0 / pi_);
	camera.rotation.Yaw = ((FastArcTan2(rotation.a * -1, rotation.b) * (180.0 / pi_)) * -1) * -1;
	camera.rotation.Roll = 0.0;

	camera.fov = km_ptr->read<float>(cached->player_controller + 0x394) * 90.f;
}

inline float ratio = (float)monitor.width / monitor.height;

Vector3 sdk::camera_manager::w2s(Vector3 pos)
{
	D3DMATRIX matrix = Matrix(camera.rotation, Vector3(0, 0, 0));

	Vector3 vAxisX = Vector3(matrix.m[0][0], matrix.m[0][1], matrix.m[0][2]);
	Vector3 vAxisY = Vector3(matrix.m[1][0], matrix.m[1][1], matrix.m[1][2]);
	Vector3 vAxisZ = Vector3(matrix.m[2][0], matrix.m[2][1], matrix.m[2][2]);

	Vector3 vDelta = pos - camera.location;

	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f) vTransformed.z = 1.f;

	if (ratio < 4.0f / 3.0f) // strech res compatibility
		ratio = 4.0f / 3.0f;

	float fov = ratio / (16.0f / 9.0f) * (float)tanf(camera.fov * pi_ / 360.0f);

	return Vector3((monitor.width / 2.0f) + vTransformed.x * (((monitor.width / 2.0f) / fov)) / vTransformed.z, (monitor.height / 2.0f) - vTransformed.y * (((monitor.width / 2.0f) / fov)) / vTransformed.z, 0);
}

Vector3 sdk::other_manager::predict(Vector3 target, Vector3 velocity, float projectile_speed, float gravity_scale, float distance)
{
	float time = distance / projectile_speed;

	target.addScaled(velocity, time);

	float gravity = std::fabs(-980.0f * gravity_scale) * 0.5f * time * time;
	target.z += gravity;

	return target;
}

std::string sdk::other_manager::get_weapon_name(uintptr_t actor)
{
	uintptr_t current_weapon = km_ptr->read<uintptr_t>(actor + 0xa20);

	uint64_t weapon_data = km_ptr->read<uint64_t>(current_weapon + 0x4d0);
	if (!weapon_data) return "";
	uint64_t fname_text = km_ptr->read<uint64_t>(weapon_data + 0x38);
	if (!fname_text) return "";
	uint32_t name_length = km_ptr->read<uint32_t>(fname_text + 0x30);
	if (!name_length) return "";
	wchar_t* WeaponName = new wchar_t[uint64_t(name_length) + 1];

	km_ptr->read_memory((PVOID)km_ptr->read<PVOID>(fname_text + 0x28), (WeaponName), name_length * sizeof(wchar_t));

	std::wstring wWeaponName(WeaponName);
	return std::string(wWeaponName.begin(), wWeaponName.end());
}

bool sdk::other_manager::is_visible(uintptr_t mesh)
{
	float submit_time = km_ptr->read<float>(mesh + 0x2e8);
	float render_time = km_ptr->read<float>(mesh + 0x2f0);

	return render_time + 0.06f >= submit_time;
}

std::string sdk::other_manager::decrypt_name(uintptr_t PlayerState)
{
	__int64 FString = km_ptr->read<__int64>(PlayerState + 0xaf0);
	int Length = km_ptr->read<int>(FString + 16);
	__int64 v6 = Length;
	if (!v6) return std::string("");
	uintptr_t FText = km_ptr->read<__int64>(FString + 8);

	wchar_t* NameBuffer = new wchar_t[Length];
	km_ptr->read_memory((PVOID)FText, NameBuffer, Length * sizeof(wchar_t));

	char v21;
	int v22;
	int i;
	int v25;
	WORD* v23;

	v21 = v6 - 1;
	if (!(DWORD)v6)
		v21 = 0;
	v22 = 0;
	v23 = (WORD*)NameBuffer;
	for (i = (v21) & 3; ; *v23++ += i & 7)
	{
		v25 = v6 - 1;
		if (!(DWORD)v6)
			v25 = 0;
		if (v22 >= v25)
			break;
		i += 3;
		++v22;
	}

	std::wstring Temp{ NameBuffer };
	return std::string(Temp.begin(), Temp.end());
}