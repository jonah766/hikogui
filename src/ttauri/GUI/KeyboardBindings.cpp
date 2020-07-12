// Copyright 2020 Pokitec
// All rights reserved.

#include "KeyboardBindings.hpp"
#include "../JSON.hpp"
#include "../command.hpp"

namespace tt {

void KeyboardBindings::loadBindings(URL url, bool system_binding)
{
    ttlet data = parseJSON(url);

    try {
        parse_assert2(data.contains("bindings"), "Missing key 'bindings' at top level.");

        ttlet binding_list = data["bindings"];
        parse_assert2(binding_list.is_vector(), "Expecting array value for key 'bindings' at top level.");

        for (auto i = binding_list.vector_begin(); i != binding_list.vector_end(); ++i) {
            ttlet binding = *i;
            parse_assert2(binding.is_map(), "Expecting object for a binding, got {}", binding);

            parse_assert2(
                binding.contains("key") && binding.contains("command"),
                "Expecting required 'key' and 'command' for a binding, got {}", binding
            );

            ttlet key_name = static_cast<std::string>(binding["key"]);
            ttlet key = KeyboardKey(key_name);

            auto command_name = static_cast<std::string>(binding["command"]);

            // Commands starting with '-' are ignored system-bindings.
            bool ignored_binding = false;
            if (command_name.size() >= 1 && command_name[0] == '-') {
                ignored_binding = true;
                command_name = command_name.substr(1);
            }

            auto command = to_command(command_name);
            if (command == command::unknown) {
                TTAURI_THROW(parse_error("Could not parse command '{}'", command_name));
            }

            if (ignored_binding) {
                addIgnoredBinding(key, command);
            } else if (system_binding) {
                addSystemBinding(key, command);
            } else {
                addUserBinding(key, command);
            }
        }

    } catch (error &e) {
        e.set<url_tag>(url);
        throw;
    }

}


}