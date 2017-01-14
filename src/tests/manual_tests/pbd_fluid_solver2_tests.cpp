// Copyright (c) 2017 Doyub Kim

#include <manual_tests.h>

#include <jet/box2.h>
#include <jet/rigid_body_collider2.h>
#include <jet/pbd_fluid_solver2.h>
#include <jet/volume_particle_emitter2.h>

using namespace jet;

JET_TESTS(PbdFluidSolver2);

JET_BEGIN_TEST_F(PbdFluidSolver2, SteadyState) {
    // Build solver
    auto solver = PbdFluidSolver2::builder()
        .withTargetDensity(1.0)
        .makeShared();

    const auto particles = solver->sphSystemData();
    const double targetSpacing = particles->targetSpacing();

    // Build emitter
    auto box = Box2::builder()
        .withLowerCorner({targetSpacing, targetSpacing})
        .withUpperCorner({1.0 - targetSpacing, 0.5})
        .makeShared();

    auto emitter = VolumeParticleEmitter2::builder()
        .withSurface(box)
        .withSpacing(targetSpacing)
        .withIsOneShot(true)
        .makeShared();

    solver->setEmitter(emitter);

    // Build collider
    auto anotherBox = Box2::builder()
        .withLowerCorner({0, 0})
        .withUpperCorner({1, 1})
        .withIsNormalFlipped(true)
        .makeShared();

    auto collider = RigidBodyCollider2::builder()
        .withSurface(anotherBox)
        .makeShared();

    solver->setCollider(collider);

    // Simulate
    for (Frame frame; frame.index < 100; ++frame) {
        solver->update(frame);

        saveParticleDataXy(solver->particleSystemData(), frame.index);
    }
}
JET_END_TEST_F
