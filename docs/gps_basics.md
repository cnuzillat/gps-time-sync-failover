# GPS Basics

## What is gpsd?
`gpsd` is a Linux service that interfaces with GPS receivers and provides data
to client applications in a structured way. It listens to the GPS device,
parses NMEA sentences, and makes information available via TCP port 2947
as JSON objects.

## Important NMEA Sentences
### $GPRMC - Recommended Minimum Specific GPS/Transit Data
Example:
- `123519` → UTC time (12:35:19)
- `A` → Status (A = valid, V = warning)
- `4807.038,N` → Latitude 48°07.038′ North
- `01131.000,E` → Longitude 11°31.000′ East
- `022.4` → Speed over ground in knots
- `084.4` → Track angle in degrees
- `230394` → Date (23 March 1994)

### $GPGGA - Global Positioning System Fix Data
Example:
- `123519` → UTC time
- `4807.038,N` → Latitude
- `01131.000,E` → Longitude
- `1` → Fix quality (0 = invalid, 1 = GPS fix, 2 = DGPS fix)
- `08` → Number of satellites
- `0.9` → Horizontal dilution of precision
- `545.4,M` → Altitude above sea level (545.4m)
