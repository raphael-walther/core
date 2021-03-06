#ifndef MESSAGE_SEARCH_H
#define MESSAGE_SEARCH_H

struct message_block;
struct message_part;
struct message_search_context;

enum message_search_flags {
	/* Skip the main header and all the MIME headers. */
	MESSAGE_SEARCH_FLAG_SKIP_HEADERS	= 0x01
};

/* The key must be given in UTF-8 charset */
struct message_search_context *
message_search_init(const char *normalized_key_utf8,
		    normalizer_func_t *normalizer,
		    enum message_search_flags flags);
void message_search_deinit(struct message_search_context **ctx);

/* Returns TRUE if key is found from input buffer, FALSE if not. */
bool message_search_more(struct message_search_context *ctx,
			 struct message_block *raw_block);
/* Same as message_search_more(), but return the decoded block. If the same
   input is being fed to multiple searches, this avoids duplicating the work
   by doing the following searches with message_search_more_decoded() */
bool message_search_more_get_decoded(struct message_search_context *ctx,
				     struct message_block *raw_block,
				     struct message_block *decoded_block_r);
/* The data has already passed through decoder. */
bool message_search_more_decoded(struct message_search_context *ctx,
				 struct message_block *block);
void message_search_reset(struct message_search_context *ctx);
/* Search a full message. Returns 1 if match was found, 0 if not,
   -1 if error (if stream_error == 0, the parts contained broken data) */
int message_search_msg(struct message_search_context *ctx,
		       struct istream *input, struct message_part *parts,
		       const char **error_r)
	ATTR_NULL(3);

#endif
