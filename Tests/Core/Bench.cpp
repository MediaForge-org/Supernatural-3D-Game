// Micro-benchmarks of the pure movement / traversal / perception functions (not a CTest test; run by hand).
// Build target: spn_core_bench. Release numbers need a Release configuration; the default presets are Debug, so these are
// DEBUG-build numbers unless configured otherwise. They say nothing about Unreal frame cost.

#include <chrono>
#include <cstdio>

#include <spn/core/GroundSteering.h>
#include <spn/core/InteractionReach.h>
#include <spn/core/JumpModel.h>
#include <spn/core/Locomotion.h>
#include <spn/core/Npc.h>
#include <spn/core/Traversal.h>

using namespace spn::core;
using Clock = std::chrono::steady_clock;

namespace {
template <class F>
void Bench(const char* name, int n, F&& f) {
    volatile double sink = 0.0;
    const auto t0 = Clock::now();
    for (int i = 0; i < n; ++i) sink = sink + f(i);
    const double ns = std::chrono::duration<double, std::nano>(Clock::now() - t0).count() / n;
    std::printf("%-28s %8.1f ns/call (%d calls)\n", name, ns, n);
}
} // namespace

int main() {
    const MovementConfig move;
    const SteeringConfig steer;
    const TraversalConfig trav;
    Vec2 v{8.0, 0.0};
    Bench("SteerVelocity", 2000000, [&](int i) {
        v = SteerVelocity(steer, v, SteeringInput{{(i & 1) ? 1.0 : 0.0, (i & 1) ? 0.0 : 1.0}, 8.0, 4.0, 8.0}, 1.0 / 60.0);
        return v.x;
    });
    Bench("ClassifyTraversal", 2000000, [&](int i) {
        return static_cast<double>(ClassifyTraversal(move, trav, TraversalObstacle{0.1 + (i % 200) * 0.01, 0.4, 10.0, 4.0, false}).move);
    });
    Bench("ComputeLocomotion", 2000000, [&](int i) {
        LocomotionInput in;
        in.motion = MotionSample{(i % 90) * 0.1, 0.0, true, -1.0};
        return ComputeLocomotion(move, in).gaitBlend;
    });
    Bench("EvaluateJump", 2000000, [&](int i) {
        return static_cast<double>(EvaluateJump(JumpAttempt{2.97, 9.81, 5.5, (i % 300) * 0.01, 0.3}));
    });
    const NpcPerceptionConfig perc;
    Bench("Perceive", 2000000, [&](int i) {
        return static_cast<double>(Perceive(perc, {0.0, 0.0}, i % 360, NpcStimulus{{(i % 40) * 0.5, 3.0}, 1.0, true}));
    });
    const ReachConfig reach;
    Bench("CheckReach", 2000000, [&](int i) {
        ReachQuery q;
        q.eye = {0.0, 0.0, 1.6};
        q.viewDirection = {1.0, 0.0, 0.0};
        q.targetCentre = {(i % 40) * 0.1, 0.5, 1.4};
        return static_cast<double>(CheckReach(reach, q));
    });
    return 0;
}
