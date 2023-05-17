
# Big and Fixed floating-point numbers in Unreal Engine 5

https://discord.gg/PwGBQBR7MM Join our Discord!

Note: Credit to Evols's SpaceKit! https://github.com/Evols/SpaceKit

Features:

- Big numbers, both floating-point and fixed-point, with user-defined precision.
- Vector (both floating-point and fixed-point), rotator and quaternion (floating-point only) types.
- Both in C++ and Blueprints, with a math/conversions library, and seamless integration (Blueprints literals, node literals, default-value) for all types.
- Seamless editor integration: you can define a property in a Blueprint/C++ actor/component, and edit it in the editor.
- Replication support
- Unit tests, that ensure the plugin works

## Installation
Unreal-FPM depends on Boost for floating-point numbers, and ttmath for fixed-point numbers.
Don't worry though, you don't have to download anything separately, it's packaged with Unreal-FPM (Boost as a separate module, ttmath inside the Unreal-FPMPrecision module).

## Configuring Unreal-FPM

You can configure the precision, by setting the number of bits of the big number type. However, keep in mind that more precision means worse performance.

For that, go to Source/Unreal-FPMPrecision/Public/PrecisionSettings.h

Unreal-FPM has unit tests, that use UE4's testing system: if you modify Unreal-FPM, remember to run them, to ensure that nothing got broken in the process.

## Using Unreal-FPM

Unreal-FPM provides big floating-point and fixed-point numbers, both in C++ and Blueprints.
You can use them directly on your actor/components: you can edit the values directly in the Unreal editor, as Unreal-FPM also provide the edition widgets.

Fixed-point numbers are numbers that have a fixed number of digits before and after the radix point.
This permits to save some space, and thus have more precision, as there is no exponent bits.

Unreal-FPM also includes vector (both floating-point and fixed-point), rotator and quaternion (floating-point only) types.
Like numbers, they come with editor widgets, so that they can be edited in the editor.

However, it doesn't provide a physics engine. Thus, you'll have to implement that yourself, if you need one.

Unreal-FPM provides C++11 custom literals for big floating-point and fixed-point numbers, respectively `_fl` and `_fx`. As an example, `const auto a = 5.24_fl;` creates an FRealFloat which value is `5.24`.
