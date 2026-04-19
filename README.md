# Multi-Container Runtime

This project involves builds a lightweight Linux container runtime in C with a long-running parent supervisor and a kernel-space memory monitor. The container runtime manages multiple containers at once, coordinates concurrent logging safely, exposes a small supervisor CLI, and includes controlled experiments related to Linux scheduling.

---

## 1. Team Information

| Name | SRN |
|------|-----|
| Krishna Manoj | PES2UG24CS236 |
| Kushi Niranjana | PES2UG24CS245 |

---

## 2. Build, Load, and Run Instructions

```bash
# Dependencies
sudo apt update && sudo apt install -y build-essential linux-headers-$(uname -r)

# Build
make

# Prepare rootfs
mkdir rootfs-base
wget https://dl-cdn.alpinelinux.org/alpine/v3.20/releases/x86_64/alpine-minirootfs-3.20.3-x86_64.tar.gz
tar -xzf alpine-minirootfs-3.20.3-x86_64.tar.gz -C rootfs-base
cp -a ./rootfs-base ./rootfs-alpha
cp -a ./rootfs-base ./rootfs-beta

# Load kernel module
sudo insmod monitor.ko
ls -l /dev/container_monitor

# Start supervisor (keep terminal open)
sudo ./engine supervisor ./rootfs-base

# In another terminal — launch containers
sudo ./engine start alpha ./rootfs-alpha /bin/sh --soft-mib 48 --hard-mib 80
sudo ./engine start beta  ./rootfs-beta  /bin/sh --soft-mib 64 --hard-mib 96

# CLI commands
sudo ./engine ps
sudo ./engine logs alpha
sudo ./engine stop alpha
sudo ./engine stop beta

# Inspect kernel logs
dmesg | tail

# Unload module
sudo rmmod monitor
```

To run workload binaries inside a container, copy them into the rootfs before launch:
```bash
cp cpu_hog ./rootfs-alpha/
```

---

## 3. Demo Screenshots

*(See report.pdf)*

| # | Demonstrates |
|---|-------------|
| 1 | Multi-container supervision — two containers under one supervisor |
| 2 | `engine ps` showing tracked container metadata |
| 3 | Log file contents via `engine logs`, producer/consumer activity |
| 4 | CLI command issued and supervisor responding over UNIX socket |
| 5 | `dmesg` showing soft-limit warning |
| 6 | `dmesg` showing hard-limit kill, `engine ps` showing state `killed` |
| 7 | Scheduling experiment — completion times at different `nice` values |
| 8 | Clean teardown — logger joined, no zombies in `ps aux` |

---

## 4. Engineering Analysis

**Isolation Mechanisms**

`clone()` is called with `CLONE_NEWPID`, `CLONE_NEWUTS`, and `CLONE_NEWNS` to give each container isolated PID, hostname, and mount namespaces. The kernel maintains separate `struct pid`, `uts_namespace`, and `mnt_namespace` structs per container. `chroot()` restricts the filesystem view to the container's own rootfs copy. `pivot_root` would be stronger (prevents `..` escape) but requires the new root to already be a mount point. The host kernel still shares the scheduler, network stack, and memory allocator with all containers — namespaces restrict visibility, not resource access.

**Supervisor and Process Lifecycle**

Linux requires a living parent to `wait()` on children — otherwise they become zombies until reparented to init. The supervisor stays alive as that persistent parent. On container exit, `SIGCHLD` fires and `sigchld_handler` calls `waitpid(-1, …, WNOHANG)` in a loop to reap all ready children and update metadata. `stop_requested` is set before any kill signal so the handler can distinguish voluntary stop (stop_requested=1) from hard-limit kill (SIGKILL + stop_requested=0). `SA_RESTART` prevents spurious `EINTR` in the accept loop.

**IPC, Threads, and Synchronization**

Two IPC paths are used. Logging (Path A) uses pipes — one producer thread per container pushes output chunks into a 16-slot bounded buffer; a single consumer thread drains it to log files. The buffer uses a `pthread_mutex_t` + two condition variables (`not_full`, `not_empty`): producers block when full, consumer blocks when empty, and `pthread_cond_broadcast` on shutdown drains cleanly. Without this, producers would race on `buffer->head` and the consumer could exit before flushing the last entries. Control (Path B) uses a UNIX domain socket — the CLI sends a fixed-size `control_request_t` and reads back a `control_response_t`. A `metadata_lock` mutex protects the container list from races between the SIGCHLD handler and the accept loop.

**Memory Management and Enforcement**

RSS counts physical pages currently in RAM for a process (`get_mm_rss` sums anonymous, file-mapped, shared, and SwapBacked pages). It excludes untouched allocations, swapped pages, and counts shared libraries once per process rather than once globally. The soft limit warns that a container is trending high but allows natural recovery — killing on first crossing would be too aggressive. The hard limit is a strict ceiling; crossing it means the container has violated its allocation contract. Enforcement belongs in kernel space because a user-space monitor is subject to scheduling delays and its signals can be caught or blocked. `send_sig(SIGKILL, …)` from the kernel is unconditional and immediate.

**Scheduling Behavior**

CFS schedules the process with the lowest `vruntime`, weighted by `nice`. In Experiment 1, the `nice 0` container finished ~3× faster than the `nice +10` container, matching the CFS weight ratio — neither starved. In Experiment 2, `io_pulse` stayed responsive against a competing `cpu_hog` at equal priority because each voluntary I/O block resets its `vruntime` to near the minimum, granting fast rescheduling on every wakeup. CFS implicitly favours I/O-bound work without explicit priority tuning.

---

## 5. Design Decisions and Tradeoffs

**Namespace isolation — `chroot` over `pivot_root`:** Simpler and sufficient for trusted workloads. Tradeoff: privileged processes can escape via `..` traversal. Justified because this project does not run adversarial containers.

**Supervisor — single-threaded select loop:** Avoids lock contention across concurrent accept threads. Tradeoff: a blocking `CMD_RUN` delays new accepts. Acceptable because `start` returns immediately by design.

**Control IPC — UNIX domain socket over FIFO:** Gives bidirectional framing and per-client connection scope needed for `CMD_RUN` and `CMD_PS`. Tradeoff: requires `unlink` cleanup on exit. FIFOs are simpler but one-way.

**Kernel monitor lock — `mutex` over `spinlock`:** `kmalloc(GFP_KERNEL)` in the ioctl path can sleep, which is illegal while holding a spinlock. `del_timer_sync` in `monitor_exit` ensures no timer fires during teardown, so the mutex is safe throughout.

**Scheduling experiments — `nice` via `setpriority`:** Unprivileged, measurable, and sufficient to demonstrate CFS weight behavior. Tradeoff: adjusts relative weight only, not hard real-time guarantees. `SCHED_FIFO` would be stronger but requires `CAP_SYS_NICE`.

---

## 6. Scheduler Experiment Results

**Experiment 1 — Two CPU-bound containers at different `nice` values**

Both containers ran `cpu_hog` over the same fixed iteration count.



Alpha finished  faster than beta, consistent with the CFS weight ratio for a significant nice difference. Neither container starved.

**Experiment 2 — CPU-bound vs. I/O-bound at equal priority**

`cpu_hog` and `io_pulse` ran concurrently at `nice 0`. Despite `cpu_hog` saturating the CPU, `io_pulse` completed all I/O rounds with low latency. Each voluntary block reset its `vruntime` to near the minimum, so it was rescheduled promptly on every wakeup — demonstrating CFS's implicit responsiveness benefit for I/O-bound workloads.
