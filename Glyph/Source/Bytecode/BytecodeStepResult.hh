#pragma once

#include <AST/AST.hh>
#include <absl/status/status.h>
#include <utility>

namespace Glyph::Bytecode
{
	struct BytecodeStepResult
	{
		AstNode const* Node {nullptr};
		absl::Status Status;

		BytecodeStepResult(AstNode const* node, absl::Status status)
			: Node(node)
			, Status(std::move(status))
		{
		}

		BytecodeStepResult()
			: Node(nullptr)
			, Status(absl::OkStatus())
		{
		}

		/*implicit */ BytecodeStepResult(absl::Status status)
			: Node(nullptr)
			, Status(std::move(status))
		{
		}

		[[nodiscard]] bool ok() const { return Status.ok(); }
	};
} // namespace Glyph::Bytecode
