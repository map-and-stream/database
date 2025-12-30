#!/bin/bash
# Script to fix pgpool2 authentication with PostgreSQL backend

echo "=== Fixing pgpool2 Authentication ==="
echo ""
echo "The error 'failed to authenticate with backend using SCRAM' means"
echo "pgpool2 cannot authenticate to PostgreSQL with the provided password."
echo ""

echo "Solution 1: Create pool_passwd file (Recommended)"
echo "----------------------------------------"
echo "This file stores the password for pgpool2 to authenticate to PostgreSQL."
echo ""
echo "Run these commands:"
echo ""
echo "1. Generate the pool_passwd file:"
echo "   sudo pg_md5 -p -u postgres /etc/pgpool2/pool_passwd"
echo "   (Enter the PostgreSQL password when prompted)"
echo ""
echo "2. Set ownership:"
echo "   sudo chown postgres:postgres /etc/pgpool2/pool_passwd"
echo "   sudo chmod 600 /etc/pgpool2/pool_passwd"
echo ""
echo "3. Enable pool_passwd in pgpool.conf:"
echo "   sudo sed -i 's/^#pool_passwd =/pool_passwd = \/etc\/pgpool2\/pool_passwd/' /etc/pgpool2/pgpool.conf"
echo ""
echo "4. Restart pgpool2:"
echo "   sudo systemctl restart pgpool2"
echo ""

echo "Solution 2: Use .pgpass file"
echo "----------------------------------------"
echo "Create ~/.pgpass file for postgres user:"
echo ""
echo "   sudo -u postgres bash -c 'echo \"127.0.0.1:5432:postgres:postgres:PASSWORD\" > ~/.pgpass'"
echo "   sudo -u postgres chmod 600 ~/.pgpass"
echo "   (Replace PASSWORD with actual PostgreSQL password)"
echo ""

echo "Solution 3: Change PostgreSQL password to match code"
echo "----------------------------------------"
echo "If you want to use password 'qazwsx' from your code:"
echo ""
echo "   sudo -u postgres psql -c \"ALTER USER postgres PASSWORD 'qazwsx';\""
echo ""

echo "Solution 4: Check current PostgreSQL password"
echo "----------------------------------------"
echo "To test if password 'qazwsx' works:"
echo "   PGPASSWORD=qazwsx psql -h 127.0.0.1 -p 5432 -U postgres -d postgres -c 'SELECT 1;'"
echo ""

echo "=== Current Configuration Check ==="
echo ""
echo "Checking if pool_passwd exists:"
if [ -f /etc/pgpool2/pool_passwd ]; then
    echo "   ✓ pool_passwd file exists"
    sudo ls -la /etc/pgpool2/pool_passwd 2>/dev/null || echo "   ✗ Cannot check permissions"
else
    echo "   ✗ pool_passwd file does NOT exist"
fi

echo ""
echo "Checking pgpool.conf for pool_passwd setting:"
sudo grep "^pool_passwd" /etc/pgpool2/pgpool.conf 2>/dev/null || echo "   pool_passwd not configured (commented out or missing)"

echo ""
echo "=== Next Steps ==="
echo "1. Determine the correct PostgreSQL password"
echo "2. Create pool_passwd file with: sudo pg_md5 -p -u postgres /etc/pgpool2/pool_passwd"
echo "3. Enable it in pgpool.conf"
echo "4. Restart pgpool2: sudo systemctl restart pgpool2"

