# guitarix-supercollider

Guitarix effects in Super Collider. This will work with SC 3.10 if using SC < 3.10 try this repo: https://github.com/gabrielsanchez/faust-guitarix-sc37

## Installation

Drag the folder into your SuperCollider Extensions folder. It's probably in /usr/share/SuperCollider/Extensions

If it doesn't work, you will need to download FAUST and run the faust2supercollider script. Running the faust2supercollider script that will generate these files.

## Example

```supercollider
(
// Multi-Band Distortion
SynthDef(\mbdistortion, {arg in, out=0, vol=16.1, pregain=20, gain=20, bass_boost=10, eq_b=0.61, eq_m=0.04, eq_h=0.89, rev=0.45, room=0.5, attack=0.002, knee=5, ratio=3, release=0.3, threshold=16, cabinet=10, cab_bass=0.8, cab_treble=0.9, dist_gain=2, drive=0.65, high_drive=1, middle_h_drive=1, middle_l_drive=1, low_drive=1;
	var sig, disto;
	sig = SoundIn.ar(in);
	disto = FaustCompressor.ar(
		Pan2.ar(
			FreeVerb.ar(
				FaustVolume.ar(
					FaustCabinetImpulseFormer.ar(
						FaustMultiBandDistortion.ar(
							FaustRolandStyle.ar(
								FaustPresenceLevel.ar(
									FaustBassbooster.ar(
										FaustAmpT2.ar(
											FaustValve.ar(sig,0.01,1),
											pregain, gain
										), bass_boost
									)
								), eq_b, eq_m, eq_h
							), dist_gain, drive, high_drive, middle_h_drive, middle_l_drive, low_drive
						), cabinet, cab_bass, cab_treble
					), vol.neg
				), rev, room
			)
		), attack, knee, ratio, release, threshold.neg
	);
	Out.ar([0,1], disto!2);
}).add;
)

b = Synth(\distortion, [\in, 0])
```


