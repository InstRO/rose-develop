//#include <ross.h>

typedef int tw_lpid;
typedef int tw_bf;
 
/* Data structure to hold state of logical process (LP) */
typedef struct {
  long long int npart;
  long long int transit;
} ctr_state;

/* Data structure to hold contents of a message/event */
typedef struct {
  long long int	npart_send;
} ctr_message;

typedef struct tw_event {
  ctr_message dummy;
} tw_event;

typedef struct tw_lp {
  int rng;
} tw_lp;

int g_tw_nlp=1;
int TW_LOC=0;

// -----------------------------------------------------------------


/* Global variables */
static unsigned int total_no_lps = 0;
static const double mean = 5.0,lookahead = 1.0;

/* Event processing routine. Perform the relevant actions when event
   m happens to object s. */
void ctr_event_handler(ctr_state * s, tw_bf * bf, ctr_message * m, tw_lp * lp) {
  /* We received the message m. Update local particle count,
     and number of particles in transit by number of received particles. */ {
    s->npart += m->npart_send;
    s->transit -= m->npart_send;
  }

  /* Generate a new event to send some number of particles to
     another process (LP) */ {
    tw_lpid destination;
    double event_time;
    long long int nsend;
    ctr_message *msg;
    tw_event *evt;
    
    /* Let destination be a random other LP */
    destination = tw_rand_integer(lp->rng, 0, total_no_lps - 1);
    if(destination < 0 || destination >= (g_tw_nlp * tw_nnodes()))
      tw_error(TW_LOC, "bad dest");
    
    /* Pick a random numbe rof particles to send out */    
    nsend = tw_rand_integer(lp->rng,0,(s->npart+1)/2);
    
    /* Draw time at which sent out particles will arrive from exponential distribution */
    event_time = tw_rand_exponential(lp->rng, mean) + lookahead;
    
    /* Create a new event, fill in how many particles to send,
       and send it*/ {
      evt = tw_event_new(destination, event_time, lp);
      msg = tw_event_data(evt);
      msg->npart_send = nsend;
      tw_event_send(evt);
    }
    
    /* Update local particle count and number of particle sin transit */ {
      s->npart -= nsend; // model-code explicitly modifying state
      s->transit += nsend; // model-code
    }
    
    /* Store number of sent particles in auxiliary data field, that can be used by
       the inverse event routine to "undo" this event */ {
      *((unsigned int *) bf) = nsend; // MS: using a ROSS feature for storing information to be used in the reverse code (here it is an int).
    }
  }
}

/* Reverse event processing routine. Should perform the inverse of ctr_event_handler() */
void ctr_event_handler_rc(ctr_state * s, tw_bf * bf, ctr_message * m, tw_lp * lp)
{
  /* We used three random numbers in forward event processing.
     Roll the random number generator back threee times */ {
    tw_rand_reverse_unif(lp->rng);
    tw_rand_reverse_unif(lp->rng);
    tw_rand_reverse_unif(lp->rng);
  }

  /* Use auxiliary data field saved by event to
     recaculate the particle counts we had */ {
    s->npart += *(unsigned int *) bf;
    s->transit -= *(unsigned int *) bf;
  }
  
  /* Update local particle counts based on number of particles that were send in
     message m */ {
    s->npart -= m->npart_send;
    s->transit += m->npart_send;
  }
}
