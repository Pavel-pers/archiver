#include "scorer.h"

ScoreTable GetScoredStudents(const Events& events, time_t score_time) {
    std::vector<const Event*> ordered_events;
    for (const Event& ei : events) {
        ordered_events.push_back(&ei);
    }

    std::sort(ordered_events.begin(), ordered_events.end(), [](const Event* l, const Event* r) {
        return std::tie(l->student_name, l->task_name, l->time) < std::tie(r->student_name, r->task_name, r->time);
    });

    bool is_closed = true;
    bool is_accepted = false;
    ScoreTable score_table;
    for (size_t i = 0; i <= ordered_events.size(); ++i) {
        if (i == ordered_events.size() ||
            (i && std::tie(ordered_events[i - 1]->student_name, ordered_events[i - 1]->task_name) !=
                      std::tie(ordered_events[i]->student_name, ordered_events[i]->task_name))) {
            if (is_closed && is_accepted) {
                score_table[ordered_events[i - 1]->student_name].insert(ordered_events[i - 1]->task_name);
            }
            is_closed = true, is_accepted = false;
        }

        if (i < ordered_events.size() && ordered_events[i]->time <= score_time) {
            switch (ordered_events[i]->event_type) {
                case EventType::MergeRequestClosed:
                    is_closed = false;
                    break;
                case EventType::MergeRequestOpen:
                    is_closed = true;
                    break;
                case EventType::CheckFailed:
                    is_accepted = false;
                    break;
                case EventType::CheckSuccess:
                    is_accepted = true;
                    break;
            }
        }
    }

    return score_table;
}