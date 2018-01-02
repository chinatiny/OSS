#! /bin/bash
INSTALL_PREFIX=/data

# ensure base directories owned
chown socorro /var/log/socorro
chown -R www-data.socorro $INSTALL_PREFIX/socorro/webapp-django/static/CACHE

# crond doesn't like files with executable bits, and doesn't load
# them.
chmod 644 /etc/cron.d/socorro
