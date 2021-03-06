#pragma once

#include <math/Grid.h>
#include <math/Vec.h>
#include <gen/Generated/Forward.h>

#include <unordered_map>
#include <vector>
#include <array>
#include <set>
#include <functional>
#include <random>
#include <string>

namespace mud
{
	using string = std::string;
	using ubool = uint8_t;

#define MUD_WFC_PROPAGATION_VEC

	enum _refl_ Result : unsigned int
	{
		kSuccess,
		kFail,
		kUnfinished,
	};

	struct _refl_ MUD_GEN_EXPORT Tile
	{
		Tile(size_t index, string name, char symmetry, int cardinality, int profile) : m_index(index), m_name(name), m_symmetry(symmetry), m_cardinality(cardinality), m_profile(profile) {}
		_attr_ size_t m_index;
		_attr_ string m_name;
		_attr_ char m_symmetry;
		_attr_ int m_cardinality;
		_attr_ int m_profile;
		std::array<int, 8> m_flips;
		std::array<uint32_t, 6> m_edges;
	};

	struct _refl_ MUD_GEN_EXPORT Tileset
	{
		_attr_ string m_name;
		_attr_ vec3 m_tile_size;
		_attr_ vec3 m_tile_scale;
		std::vector<Tile> m_tiles;
		std::vector<Tile> m_tiles_flip;
		std::vector<double> m_weights;
		_attr_ uint16_t m_num_tiles;

		_constr_ Tileset() {}
		Tile* tile(const string& name) { for(Tile& tile : m_tiles) if(tile.m_name == name) return &tile; return nullptr; }
		int flip(int tile, uint8_t flip) const { return m_tiles_flip[tile].m_flips[flip]; }
	};

	using RandomDouble = std::function<double()>;
	using ValidCoord = std::function<bool(int, int, int)>;
	using Propagator = std::function<void(Wave&)>;

	struct DoubleGenerator
	{
		std::random_device device;
		std::mt19937 generator;
		std::uniform_real_distribution<double> distribution;
		double next() { return distribution(generator); }
		DoubleGenerator() : device(), generator(device()), distribution(0.0, 1.0) {}
	};

	struct _refl_ MUD_GEN_EXPORT Wave
	{
		Wave();
		Wave(uint16_t states, uint16_t width, uint16_t height, uint16_t depth, bool periodic);

		uint16_t m_width, m_height, m_depth;
		bool m_periodic;

		std::vector<double> m_states;
		std::vector<string> m_pattern_names; // debug
		array_3d<std::vector<ubool>> m_wave;
		std::vector<uvec3> m_changes;

		bool m_stabilized = true;
		bool m_solved = false;

		RandomDouble m_random_double;
		ValidCoord m_valid_coord;
		Propagator m_propagate;

		/*double m_logT;
		std::vector<double> m_log_weights;

		void setup()
		{
		m_logT = log(m_num_states);

		m_log_weights.resize(m_num_states);
		for(int t = 0; t < m_num_states; t++)
		m_log_weights[t] = log(m_state_weights[t]);

		}*/

		void clear();
		void add_foundation(uint16_t tile);
		Result find_lowest_entropy(uvec3& coord);
		Result observe();
		void propagate();

		_meth_ Result solve(size_t limit);
	};

	struct _refl_ MUD_GEN_EXPORT WaveTileset : public Tileset
	{
		array_3d<ubool> m_propagator[6];

		_constr_ WaveTileset();
		void initialize();
		void connect(int left, int right, bool horizontal);
		void finalize();

		array_3d<ubool>& side(SignedAxis axis) { return m_propagator[size_t(axis)]; }
	};

	struct _refl_ MUD_GEN_EXPORT TileWave : public Wave
	{
		TileWave();
		_constr_ TileWave(WaveTileset& tileset, uint16_t width, uint16_t height, uint16_t depth, bool periodic);
	};

	MUD_GEN_EXPORT _func_ void parse_json_tileset(const string& path, const string& subset, Tileset& outputTileset);
	MUD_GEN_EXPORT _func_ void parse_json_wave_tileset(const string& path, const string& subset, WaveTileset& outputTileset);

	struct VoxelTile
	{
		size_t m_pattern;
		uint8_t m_profile;
	};

	struct RGBA
	{
		uint8_t r, g, b, a;
	};
	static_assert(sizeof(RGBA) == 4, "");
	inline bool operator==(RGBA x, RGBA y) { return x.r == y.r && x.g == y.g && x.b == y.b && x.a == y.a; }

	using ColorIndex = uint8_t; // tile index or color index. If you have more than 255, don't.
	using ColorPalette = std::vector<RGBA>;
	using ColorPattern = std::vector<ColorIndex>;
	using PatternHash = uint64_t; // Another representation of a Pattern.
	using PatternPrevalence = std::unordered_map<PatternHash, size_t>;
	using PatternIndex = uint16_t;

	const auto kInvalidIndex = static_cast<size_t>(-1);
	const auto kInvalidHash = static_cast<PatternHash>(-1);

	const size_t MAX_COLORS = 1 << (sizeof(ColorIndex) * 8);

	using OverlapGraphics = array_3d<std::vector<ColorIndex>>;

	struct PalettedImage
	{
		size_t                  width, height;
		std::vector<ColorIndex> data; // width * height
		ColorPalette            palette;

		ColorIndex at_wrapped(size_t x, size_t y) const
		{
			return data[width * (y % height) + (x % width)];
		}
	};

	class Patternset
	{
	public:
		Patternset(int n, const PatternPrevalence& hashed_patterns, const ColorPalette& palette, PatternHash foundation_pattern);

		int m_n;
		array_3d<std::vector<std::vector<PatternIndex>>> m_propagator;
		std::vector<ColorPattern> m_patterns;
		std::vector<double> m_weights;
	};
}
