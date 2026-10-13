#ifndef STAGE_PROGRESS_H
#define STAGE_PROGRESS_H

/**
 * Tracks the player's progress across stages for the current run.
 *
 * The state lives only in memory: it advances when a flag is captured and is
 * not persisted to disk, so a full application restart begins again at stage 1
 * with zero captured flags.
 */
class StageProgress {
public:
    /**
     * The stage currently being played, starting at 1.
     */
    int getStage() const;

    /**
     * Total number of flags captured so far, summed across stages.
     */
    int getFlags() const;

    /**
     * Register that a flag was captured: the flag counter grows and the run
     * moves on to the next stage.
     */
    void collectFlag();

    /**
     * Forget all progress: back to stage 1 with zero captured flags.
     */
    void reset();

private:
    int stage = 1;
    int flags = 0;
};

#endif /* STAGE_PROGRESS_H */
