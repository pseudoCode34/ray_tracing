#include "example.hpp"

#include <array>
#include <cstdlib>
#include <fmt/core.h>
#include <ranges>
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
constexpr auto COMMAND_TABLE = std::to_array<CommandEntry>(
	{{"sphere", "\tGenerate a sphere", draw_sphere}});

void print_help() {
	for (const auto &[action, help, _] : COMMAND_TABLE)
		fmt::println("\n{}\n{}", action, help);
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

	Eigen::initParallel();
	const auto *result
		= std::ranges::find(COMMAND_TABLE, argv[1], &CommandEntry::subcommand);
	if (result == std::ranges::end(COMMAND_TABLE)) {
		spdlog::error("Unknown command line option '{}'", argv[1]);
		return EXIT_FAILURE;
	}

	spdlog::info("Performing {}", argv[1]);
	result->callback();
	return EXIT_SUCCESS;
}
