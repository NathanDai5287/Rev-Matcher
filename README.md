# Rev Matcher for ND Miata

A real-time rev matching calculator for ND generation Mazda Miata transmissions that reads engine RPM and vehicle speed via OBD-II to calculate optimal RPM targets for smooth upshifts and downshifts.

## Features

- **Real-time RPM and Speed Monitoring**: Reads live engine RPM and vehicle speed via OBD-II
- **Rev Drop Calculations**: Shows target RPM after upshifting to time shifts perfectly
- **Rev Match Calculations**: Shows target RPM before downshifting for smooth rev matching
- **Automatic Gear Detection**: Calculates current gear based on speed and RPM ratios
- **Live Display**: Continuous updates showing current gear, target RPMs, and percentage changes

## Hardware Requirements

- ND Mazda Miata (2016+)
- ELM327-compatible OBD-II adapter
- USB-to-serial cable connection (`/dev/tty.usbserial`)

## Transmission Data

Configured for ND Miata 6-speed manual transmission:
- **Final Drive**: 2.866:1
- **Gear Ratios**:
  - 1st: 5.087:1
  - 2nd: 2.991:1
  - 3rd: 2.035:1
  - 4th: 1.594:1
  - 5th: 1.286:1
  - 6th: 1.000:1

## Tire Configuration

Default setup for 205/45R17 tires:
- Wheel diameter: 17 inches
- Sidewall height: 92.25mm (45% of 205mm)
- Total diameter: ~24.26 inches

## Build and Run

```bash
gcc main.c -o rev_matcher
./rev_matcher
```

## Output Format

```
Current Gear - Current RPM
Gear ↑ Next Gear: Target RPM (-Drop, -Percentage%)
Gear ↓ Previous Gear: Target RPM (+Rise, +Percentage%)
```

Example output:
```
3 - 4200
3 ↑ 4: 3290 (-910, -21%)
3 ↓ 2: 6176 (+1976, +47%)
```

## Customization

To adapt for different vehicles or tire sizes, modify:
- `gear_utils.h`: Gear ratios and final drive ratio
- `tires.h`: Wheel diameter and tire dimensions
- `main.c`: Serial port device path

## Notes

- Requires root/administrator privileges for serial port access
- Designed for track/performance driving assistance
- Updates every 200ms for responsive real-time feedback
