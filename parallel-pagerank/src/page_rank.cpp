void pageRank(Graph g, double* solution, double damping, double convergence) {
    // ...
    while (!converged && iter < MAXITER) {
        iter++;
        broadcastScore = 0.0;
        globalDiff = 0.0;
        for (int i = 0; i < numNodes; ++i) {
            score_new[i] = 0.0;

            if (outgoing_size(g, i) == 0) {
                broadcastScore += score_old[i];
            }
            const Vertex* in_begin = incoming_begin(g, i);
            const Vertex* in_end = incoming_end(g, i);
            for (const Vertex* v = in_begin; v < in_end; ++v) {
                score_new[i] += score_old[*v] / outgoing_size(g, *v);
            }
            score_new[i] = damping * score_new[i] + (1.0 - damping) * equal_prob;
        }
        for (int i = 0; i < numNodes; ++i) {
            score_new[i] += damping * broadcastScore * equal_prob;
            globalDiff += std::abs(score_new[i] - score_old[i]);
        }
        converged = (globalDiff < convergence);
        std::swap(score_new, score_old);
    }
    // ...
}