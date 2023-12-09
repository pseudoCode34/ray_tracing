#include "example.hpp"

#include <array>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <string_view>

namespace raytracing {
using COMMAND_FUNCTION = void (*)();

struct CommandEntry {
	std::string_view subcommand, usage;
	COMMAND_FUNCTION callback;
};

// You can add more command line options to this program by adding another entry
// to the array below. Each item in the ray is a string followed by a function
// to be called when that string appears on the command line.
constexpr auto COMMAND_TABLE = std::to_array<CommandEntry>({
	{"test",
	 "\tRuns a series of unit tests that generate sample PNG images.\n",
	 Imager::run_all},
	{"spheroid",
	 "\tGenerates an image of a spheroid.\n",
	 Imager::spheroid_test},
	{"chessboard",
	 "\tChess board and transparent spheres.\n",
	 Imager::draw_chess_board},
	{"block",
	 "\tConcrete block with a sphere casting a shadow on it.\n",
	 Imager::block_test},
	{"kaleid",
	 "\tKaleidoscope using three mirrors.\n",
	 Imager::kaleidoscope_test},
	{"multisphere", "\tGenerate 2 spheres", Imager::multiple_sphere_test},
	{"sphere", "\tGenerate a sphere", Imager::sphere_test},
});

void print_help() {
	fmt::println("Ray Tracer demo - Copyright (C) 2013 by Don Cross.\n"
				 "For more info, see: http://cosinekitty.com/raytrace\n\n"
				 "The following command line options are supported:\n");

	for (const auto &[verb, help, _] : COMMAND_TABLE)
		fmt::println("\n{}\n{}", verb, help);
	fmt::println("");
}

} // namespace raytracing

int main(int argc, const char *argv[]) {
	if (argc == 1) {
		// No command line arguments are present, so display usage text.
		raytracing::print_help();
		return EXIT_FAILURE;
	}

	using raytracing::COMMAND_TABLE, raytracing::CommandEntry;
	const auto *result
		= std::ranges::find(COMMAND_TABLE, argv[1], &CommandEntry::subcommand);
	if (result == std::ranges::end(COMMAND_TABLE)) {
		auto main_logger = spdlog::basic_logger_mt("main", "logs/main.txt");
		main_logger->error("Unknown command line option '{}'", argv[1]);
		return EXIT_FAILURE;
	}

	fmt::println("Performing {}", argv[1]);
	result->callback();
	return EXIT_SUCCESS;
}
