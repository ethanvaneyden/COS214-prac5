# CampusGuard

## Short description

A command-line emergency-response coordination platform for a university campus. Operators register incidents, dispatch response teams, and run multi-step emergency workflows from a single interface.

## Features

- Register incidents with type, location, description, and severity
- Dispatch the required response unit based on incident type
- Coordinate four response units (Security, Medical, Maintenance, Communications) through a mediator, so they never depend directly on each other
- Full state-based logic for incidents: Reported, Dispatched, Resolved, Cancelled, with illegal transitions rejected
- Multi-step evacuation workflow that triggers the alarm, broadcasts to all units, and dispatches a response unit in a single operator action
- Integrate with the legacy campus siren system through an adapter

## Build and run

It is recommended to run this program with Docker. The provided Docker environment contains all tools required to build, run, debug, and investigate the program.

### Using Docker Compose

Assuming Docker is installed and running:

```bash
docker compose up --build -d
docker compose attach campusguard
```

Alternatively, for a one-off interactive session:

```bash
docker compose run --rm --build campusguard
```

> **Note:** `docker compose up` does not attach stdin to a single container by design. The `-d` flag starts the service in the background and `docker compose attach` connects the terminal so the CLI accepts input.

To stop the container:

```bash
docker compose down
```

### Building the image locally

If you want to build the Docker image from the provided Dockerfile:

```bash
docker build -t campusguard .
docker run -it --rm campusguard
```

## Debugging

The Docker image includes GDB with debug symbols enabled during compilation.

To run CampusGuard using GDB:

```bash
docker compose run --rm campusguard gdb ./campusguard
```

## Valgrind

Valgrind is included in the Docker image for memory checking.

To run the program with Valgrind:

```bash
docker compose run --rm campusguard make valgrind
```

The `valgrind` Makefile target runs:

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./campusguard
```

## Makefile

The project includes a Makefile with the following targets:

| Target | Description |
|--------|-------------|
| `make` | Builds the CampusGuard executable |
| `make clean` | Removes generated build and object files |
| `make valgrind` | Runs CampusGuard with Valgrind |
| `make test` | Runs the static pattern tests |

The Docker image contains the required C++ compiler, Make, GDB and Valgrind.
