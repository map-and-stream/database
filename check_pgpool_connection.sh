#!/bin/bash
# Script to check pgpool2 status and test connection

echo "=== pgpool2 Service Status ==="
systemctl is-active pgpool2 2>/dev/null && echo "Status: ACTIVE" || echo "Status: INACTIVE/FAILED"
echo ""

echo "=== Process Check ==="
if pgrep -x pgpool > /dev/null; then
    echo "✓ pgpool process is running"
    ps aux | grep "[p]gpool" | head -3
else
    echo "✗ pgpool process is NOT running"
fi
echo ""

echo "=== Port Check ==="
if netstat -tlnp 2>/dev/null | grep -q ":9999" || ss -tlnp 2>/dev/null | grep -q ":9999"; then
    echo "✓ Port 9999 is listening"
    netstat -tlnp 2>/dev/null | grep 9999 || ss -tlnp 2>/dev/null | grep 9999
else
    echo "✗ Port 9999 is NOT listening"
fi
echo ""

echo "=== Connection Test ==="
echo "Testing connection to pgpool2 (port 9999)..."
if psql -h 127.0.0.1 -p 9999 -U postgres -d postgres -c "SELECT version();" 2>&1 | grep -q "PostgreSQL"; then
    echo "✓ Connection successful!"
    psql -h 127.0.0.1 -p 9999 -U postgres -d postgres -c "SELECT version();" 2>&1 | head -3
else
    echo "✗ Connection failed"
    psql -h 127.0.0.1 -p 9999 -U postgres -d postgres -c "SELECT 1;" 2>&1 | head -5
fi
echo ""

echo "=== pgpool2 Information (if connected) ==="
psql -h 127.0.0.1 -p 9999 -U postgres -d postgres -c "SHOW pool_status;" 2>&1 | head -10
echo ""

echo "=== Backend Nodes Status ==="
psql -h 127.0.0.1 -p 9999 -U postgres -d postgres -c "SHOW pool_nodes;" 2>&1 | head -10
echo ""

echo "=== Direct PostgreSQL Connection Test ==="
echo "Testing direct connection to PostgreSQL (port 5432)..."
if PGPASSWORD=qazwsx psql -h 127.0.0.1 -p 5432 -U postgres -d postgres -c "SELECT 1;" 2>&1 | grep -q "1"; then
    echo "✓ Direct PostgreSQL connection works"
else
    echo "✗ Direct PostgreSQL connection failed"
    PGPASSWORD=qazwsx psql -h 127.0.0.1 -p 5432 -U postgres -d postgres -c "SELECT 1;" 2>&1 | head -3
fi
echo ""

