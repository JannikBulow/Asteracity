// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_ADDONS_INPUT_SYSTEM_H
#define ASTERACITY_ENGINE_ADDONS_INPUT_SYSTEM_H

#include "engine/addons/input/binding.h"

#include <engine/world/system.h>

#include <tuple>
#include <vector>

namespace engine::input {
    template<class T>
    concept HasCombine = requires(typename T::value_type a, typename T::value_type b) {
        { T::combine(a, b) } -> std::same_as<T>;
    };

    template<class ActionSet>
    class InputSystem : public ISystem {
    public:
        using Actions = typename ActionSet::types;

        explicit InputSystem(backend::IInputProvider& inputProvider)
            : mInputProvider(inputProvider) {}

        template<class Action>
        void bind(Binding<typename Action::value_type> binding) {
            bindings<Action>().bindings.push_back(std::move(binding));
        }

        template<class Action>
        void clearBindings() {
            bindings<Action>().bindings.clear();
        }

        template<class Action>
        const typename Action::value_type& value() {
            return state<Action>().value;
        }

        template<std::same_as<bool> Action>
        bool pressed() const {
            const auto& s = state<Action>();
            return !s.previousValue && s.value;
        }

        template<std::same_as<bool> Action>
        bool released() const {
            const auto& s = state<Action>();
            return s.previousValue && !s.value;
        }

        void update(Engine& engine, float dt) override {
            forEachType<Actions>([this]<class Action> {
                auto& s = state<Action>();
                s.previousValue = s.value;
                s.value = evaluate<Action>();
            });
        }

    private:
        template<class Action>
        struct HasCombineFunction {
            static constexpr bool value = false;
        };

        template<HasCombine Action>
        struct HasCombineFunction<Action> {
            static constexpr bool value = true;
        };

        template<class Action>
        struct ActionState {
            using value_type = typename Action::value_type;

            value_type value{};
            value_type previousValue{};
        };

        template<class ActionTuple>
        struct ActionStates;

        template<class... Actions>
        struct ActionStates<std::tuple<Actions...>> {
            std::tuple<ActionState<Actions>...> values;
        };

        template<class Action>
        struct ActionBindings {
            using value_type = typename Action::value_type;

            std::vector<Binding<value_type>> bindings;
        };

        template<class ActionTuple>
        struct ActionBindingsStorage;

        template<class... Actions>
        struct ActionBindingsStorage<std::tuple<Actions...>> {
            std::tuple<ActionBindings<Actions>...> values;
        };

        backend::IInputProvider& mInputProvider;

        ActionStates<Actions> mStates;
        ActionBindingsStorage<Actions> mBindings;

        template<class Action>
        ActionState<Action>& state() {
            return std::get<ActionState<Action>>(mStates.values);
        }

        template<class Action>
        const ActionState<Action>& state() const {
            return std::get<ActionState<Action>>(mStates.values);
        }

        template<class Action>
        ActionBindings<Action>& bindings() {
            return std::get<ActionBindings<Action>>(mBindings.values);
        }

        template<class Action>
        const ActionBindings<Action>& bindings() const {
            return std::get<ActionBindings<Action>>(mBindings.values);
        }

        template<class Tuple, class Function>
        void forEachType(Function&& function) {
            []<class... Actions>(std::tuple<Actions...>*, Function&& function) {
                (function.template operator()<Actions>(), ...);
            }(static_cast<Tuple*>(nullptr), std::forward<Function>(function));
        }

        template<class Action>
        typename Action::value_type evaluate() const {
            const auto& actionBindings = bindings<Action>();

            typename Action::value_type result{};

            for (const auto& binding : actionBindings.bindings) {
                if constexpr (HasCombineFunction<Action>::value) {
                    result = Action::combine(result, binding.evaluate(mInputProvider));
                } else {
                    result = binding.evaluate(mInputProvider);
                }
            }

            return result;
        }
    };
}

#endif //ASTERACITY_ENGINE_ADDONS_INPUT_SYSTEM_H
