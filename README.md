# table-stdio

The *stdio* table is conceptually similar to `table-socketmap` in
OpenSMTPD-extras: it allows to write programs that implements the
tables api speaking a simple text-based protocol.

Unlike `table-socketmap`, it provides a better protocol that's similar
to the one used by the filters.

The end goal is to have OpenSMTPD speak natively this protocol and
thus to deprecate this repository.  In the meantime, it allows to
write and test new table implementations in any programming language
without changing smtpd itself.

It works by "wrapping" the new tables and speaking with smtpd the
legacy protocol.  Here's an example usage with
[table-passwd][table-passwd]:

	table virtuals proc:"stdio /usr/local/libexec/smtpd/table-passwd /etc/mail/users"

[table-passwd]: https://github.com/OpenSMTPD/table-passwd

Please note that the legacy tables in OpenSMTPD-extras implement the
old protocol and so they don't need table-stdio.  The new
implementations are now in separate repositories and share the same
name, usage and configuration with the old ones to ease the
transition.
