
# SpaceKit - Big floating-point numbers in Unreal Engine 4

SpaceKit is an Unreal Engine 4 plugin that adds support for big floating-point and fixed-point numbers.
Vanilla UE4 only allows float and double as Uproperties, with limited support for double (no Blueprints support) ; so with this plugin, you can use way more precise numbers.
It was originally made for a space simulator (hence the name) that could handle galactic distances, but then became a separate plugin.

Features:

- Big numbers, both floating-point and fixed-point, with user-defined precision.
- Vector (both floating-point and fixed-point), rotator and quaternion (floating-point only) types.
- Both in C++ and Blueprints, with a math/conversions library, and seamless integration (Blueprints literals, node literals, default-value) for all types.
- Seamless editor integration: you can define a property in a Blueprint/C++ actor/component, and edit it in the editor.
- Replication support
- Unit tests, that ensure the plugin works

## Installation

Installing SpaceKit is pretty straight-forward, as it works juste like any UE4 plugin: create a Plugins folder into your project, clone this repository inside it, generate the Visual Studio project files of your project, add this plugin to your project, and here you go!

SpaceKit depends on Boost for floating-point numbers, and ttmath for fixed-point numbers.
Don't worry though, you don't have to download anything separately, it's packaged with SpaceKit (Boost as a separate module, ttmath inside the SpaceKitPrecision module).

## Configuring SpaceKit

You can configure the precision, by setting the number of bits of the big number type. However, keep in mind that more precision means worse performance.

For that, go to Source/SpaceKitPrecision/Public/PrecisionSettings.h

SpaceKit has unit tests, that use UE4's testing system: if you modify SpaceKit, remember to run them, to ensure that nothing got broken in the process.

## Using SpaceKit

SpaceKit provides big floating-point and fixed-point numbers, both in C++ and Blueprints.
You can use them directly on your actor/components: you can edit the values directly in the Unreal editor, as SpaceKit also provide the edition widgets.

Fixed-point numbers are numbers that have a fixed number of digits before and after the radix point.
This permits to save some space, and thus have more precision, as there is no exponent bits.

SpaceKit also includes vector (both floating-point and fixed-point), rotator and quaternion (floating-point only) types.
Like numbers, they come with editor widgets, so that they can be edited in the editor.

However, it doesn't provide a physics engine. Thus, you'll have to implement that yourself, if you need one.

SpaceKit provides C++11 custom literals for big floating-point and fixed-point numbers, respectively `_fl` and `_fx`. As an example, `const auto a = 5.24_fl;` creates an FRealFloat which value is `5.24`.
