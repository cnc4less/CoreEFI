// Generated code, don't edit

static GPIO::PinDef pins[GPIO::MaxPins] = {
	{ 0, MYNAME(GPIO::Injector1), PinModeOutput, 0, 0 }, // 0
	{ 0, MYNAME(GPIO::Injector2), PinModeOutput, 0, 0 }, // 1
	{ 0, MYNAME(GPIO::Injector3), PinModeOutput, 0, 0 }, // 2
	{ 0, MYNAME(GPIO::Injector4), PinModeOutput, 0, 0 }, // 3
	{ 0, MYNAME(GPIO::Injector5), PinModeOutput, 0, 0 }, // 4
	{ 0, MYNAME(GPIO::Injector6), PinModeOutput, 0, 0 }, // 5
	{ 0, MYNAME(GPIO::Injector7), PinModeOutput, 0, 0 }, // 6
	{ 0, MYNAME(GPIO::Injector8), PinModeOutput, 0, 0 }, // 7
	{ 0, MYNAME(GPIO::Spark1), PinModeOutput, 0, 0 }, // 8
	{ 0, MYNAME(GPIO::Spark2), PinModeOutput, 0, 0 }, // 9
	{ 0, MYNAME(GPIO::Spark3), PinModeOutput, 0, 0 }, // 10
	{ 0, MYNAME(GPIO::Spark4), PinModeOutput, 0, 0 }, // 11
	{ 0, MYNAME(GPIO::Spark5), PinModeOutput, 0, 0 }, // 12
	{ 0, MYNAME(GPIO::Spark6), PinModeOutput, 0, 0 }, // 13
	{ 0, MYNAME(GPIO::Spark7), PinModeOutput, 0, 0 }, // 14
	{ 0, MYNAME(GPIO::Spark8), PinModeOutput, 0, 0 }, // 15
	{ 12, MYNAME(GPIO::Led1), PinModeOutput, 0, 0 }, // 16
	{ 13, MYNAME(GPIO::Led2), PinModeOutput, 0, 0 }, // 17
	{ 14, MYNAME(GPIO::Led3), PinModeOutput, 0, 0 }, // 18
	{ 15, MYNAME(GPIO::Led4), PinModeOutput, 0, 0 }, // 19
	{ 41, MYNAME(GPIO::IsBrakeOn), PinModeInput, 0, 0 }, // 20
	{ 47, MYNAME(GPIO::IsSignalLeft), PinModeInput, 0, 0 }, // 21
	{ 49, MYNAME(GPIO::IsSignalRight), PinModeInput, 0, 0 }, // 22
	{ 32, MYNAME(GPIO::IsParkingOn), PinModeInput, 0, 0 }, // 23
	{ 40, MYNAME(GPIO::IsLoBeamOn), PinModeInput, 0, 0 }, // 24
	{ 0, MYNAME(GPIO::IsHiBeamOn), PinModeInput, 0, 0 }, // 25
	{ 44, MYNAME(GPIO::IsKeyAcc), PinModeInput, 0, 0 }, // 26
	{ 46, MYNAME(GPIO::IsKeyOn), PinModeInput, 0, 0 }, // 27
	{ 48, MYNAME(GPIO::IsCranking), PinModeInput, 0, 0 }, // 28
	{ 43, MYNAME(GPIO::IsClimateOn), PinModeInput, 0, 0 }, // 29
	{ 45, MYNAME(GPIO::IsHornOn), PinModeInput, 0, 0 }, // 30
	{ 37, MYNAME(GPIO::IsTransNeutral), PinModeInput, 0, 0 }, // 31
	{ 39, MYNAME(GPIO::IsTransReverse), PinModeInput, 0, 0 }, // 32
	{ 0, MYNAME(GPIO::IsHazardsOn), PinModeInput, 0, 0 }, // 33
	{ 0, MYNAME(GPIO::IsInteriorOn), PinModeInput, 0, 0 }, // 34
	{ 0, MYNAME(GPIO::IsClusterBright), PinModeInput, 0, 0 }, // 35
	{ 0, MYNAME(GPIO::IsFanSwitch1), PinModeInput, 0, 0 }, // 36
	{ 0, MYNAME(GPIO::IsFanSwitch2), PinModeInput, 0, 0 }, // 37
	{ 0, MYNAME(GPIO::IsMenuButton1), PinModeInput, 0, 0 }, // 38
	{ 9, MYNAME(GPIO::IsMenuButton2), PinModeInput, 0, 0 }, // 39
	{ 0, MYNAME(GPIO::RelayWipers), PinModeOutput, 0, 0 }, // 40
	{ 0, MYNAME(GPIO::RelayClimate), PinModeOutput, 0, 0 }, // 41
	{ 0, MYNAME(GPIO::RelayStarter), PinModeOutput, 0, 0 }, // 42
	{ 0, MYNAME(GPIO::RelayHorn), PinModeOutput, 0, 0 }, // 43
	{ 0, MYNAME(GPIO::RelayHiBeam), PinModeOutput, 0, 0 }, // 44
	{ 31, MYNAME(GPIO::RelayLoBeam), PinModeOutput, 0, 0 }, // 45
	{ 27, MYNAME(GPIO::RelaySignalLeft), PinModeOutput, 0, 0 }, // 46
	{ 25, MYNAME(GPIO::RelaySignalRight), PinModeOutput, 0, 0 }, // 47
	{ 23, MYNAME(GPIO::RelayParkingLeft), PinModeOutput, 0, 0 }, // 48
	{ 0, MYNAME(GPIO::RelayParkingRight), PinModeOutput, 0, 0 }, // 49
	{ 29, MYNAME(GPIO::RelayReverseLights), PinModeOutput, 0, 0 }, // 50
	{ 33, MYNAME(GPIO::RelayBrakeLightLeft), PinModeOutput, 0, 0 }, // 51
	{ 35, MYNAME(GPIO::RelayBrakeLightRight), PinModeOutput, 0, 0 }, // 52
	{ 0, MYNAME(GPIO::RelayBrakeSignalLeft), PinModeOutput, 0, 0 }, // 53
	{ 0, MYNAME(GPIO::RelayBrakeSignalRight), PinModeOutput, 0, 0 }, // 54
	{ 0, MYNAME(GPIO::RelayFan1), PinModeOutput|PinModePWM, 0, 0 }, // 55
	{ 0, MYNAME(GPIO::RelayFan2), PinModeOutput|PinModePWM, 0, 0 }, // 56
	{ 0, MYNAME(GPIO::RelayEPAS1), PinModeOutput|PinModePWM, 0, 0 }, // 57
	{ 0, MYNAME(GPIO::RelayEPAS2), PinModeOutput|PinModePWM, 0, 0 }, // 58
	{ 0, MYNAME(GPIO::RelayGauge1), PinModeOutput|PinModePWM, 0, 0 }, // 59
	{ 0, MYNAME(GPIO::RelayGauge2), PinModeOutput|PinModePWM, 0, 0 }, // 60
	{ 0, MYNAME(GPIO::AnalogRadiatorTemp), PinModeAnalog, 0, 0 }, // 61
	{ 0, MYNAME(GPIO::AnalogFuel), PinModeAnalog, 0, 0 }, // 62
	{ 0, MYNAME(GPIO::AnalogAMPS1), PinModeAnalog, 0, 0 }, // 63
	{ 0, MYNAME(GPIO::AnalogHEGO1), PinModeAnalog, 0, 0 }, // 64
	{ 0, MYNAME(GPIO::AnalogHEGO2), PinModeAnalog, 0, 0 }, // 65
	{ 0, MYNAME(GPIO::AnalogBAR), PinModeAnalog, 0, 0 }, // 66
	{ 0, MYNAME(GPIO::AnalogEGR), PinModeAnalog, 0, 0 }, // 67
	{ 0, MYNAME(GPIO::AnalogVCC), PinModeAnalog, 0, 0 }, // 68
	{ 0, MYNAME(GPIO::AnalogDEC), PinModeAnalog, 0, 0 }, // 69
	{ 0, MYNAME(GPIO::AnalogTPS), PinModeAnalog, 0, 0 }, // 70
	{ 0, MYNAME(GPIO::AnalogVSS), PinModeInput|PinModeEdge, 0, 0 }, // 71
	{ 0, MYNAME(GPIO::AnalogECT), PinModeAnalog, 0, 0 }, // 72
	{ 0, MYNAME(GPIO::AnalogMAF), PinModeAnalog, 0, 0 }, // 73
	{ 0, MYNAME(GPIO::AnalogACT), PinModeAnalog, 0, 0 }, // 74
};

