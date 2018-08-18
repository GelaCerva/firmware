// controls the relay
//

struct ControllerConfig
{
    double setpoint;
    double band;

    double max_range;
    double min_range;

    int relayPin;
    int ledPin;
};

char state = 'I';

void controlerStateMachine(struct ControllerConfig* config, double t){

    double upper_t = config->setpoint + config->band;
    double lower_t = config->setpoint - config->band;

    if ((t > config->max_range) || (config->min_range > t))
        state = 'F'; // fail

    switch (state) {

        case 'I':
            // Inital
            digitalWrite(config->relayPin, LOW);
            digitalWrite(config->ledPin, HIGH);
            state = 'L';
            break;

        case 'L':
            digitalWrite(config->relayPin, LOW);
            if(t > upper_t) state = 'H';
            break;

        case 'H':
            digitalWrite(config->relayPin, HIGH);
            if(lower_t > t) state = 'L';
            break;

        default:
            // fail-safe
            digitalWrite(config->relayPin, LOW);
            digitalWrite(config->ledPin, LOW);
            if ((t < config->max_range) && (config->min_range < t))
                state = 'I'; // recover
            break;
    }
}

