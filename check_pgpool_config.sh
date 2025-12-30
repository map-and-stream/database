#!/bin/bash
# Script to check pgpool2 configuration

echo "=== Checking pgpool2 Configuration ==="
echo ""

echo "1. Main Configuration File (/etc/pgpool2/pgpool.conf):"
echo "   Key settings to check:"
echo "   - listen_addresses (should be '*' or '0.0.0.0' or 'localhost')"
echo "   - port (should be 9999)"
echo "   - backend_hostname0 (PostgreSQL server address)"
echo "   - backend_port0 (PostgreSQL port, usually 5432)"
echo "   - backend_weight0 (backend weight)"
echo "   - num_init_children (number of child processes)"
echo ""

echo "2. Backend Configuration (most important for your error):"
sudo grep -E "^backend_hostname|^backend_port|^backend_weight|^backend_data_directory" /etc/pgpool2/pgpool.conf 2>/dev/null || echo "   Cannot read config file (needs sudo)"
echo ""

echo "3. Network Configuration:"
sudo grep -E "^listen_addresses|^port" /etc/pgpool2/pgpool.conf 2>/dev/null || echo "   Cannot read config file (needs sudo)"
echo ""

echo "4. Authentication Configuration:"
echo "   Checking pool_hba.conf:"
sudo grep -v "^#" /etc/pgpool2/pool_hba.conf 2>/dev/null | grep -v "^$" | head -10 || echo "   Cannot read pool_hba.conf"
echo ""

echo "5. PostgreSQL Backend Status:"
psql -h 127.0.0.1 -p 5432 -U postgres -d postgres -c "SELECT version();" 2>&1 | head -3
echo ""

echo "6. pgpool2 Process Status:"
ps aux | grep "[p]gpool" | head -3
echo ""

echo "7. Network Ports:"
netstat -tlnp 2>/dev/null | grep -E ":(5432|9999)" || ss -tlnp 2>/dev/null | grep -E ":(5432|9999)"
echo ""

echo "=== Common Issues and Fixes ==="
echo ""
echo "If you see 'failed to create a backend connection', check:"
echo "1. backend_hostname0 should point to PostgreSQL server (usually 'localhost' or '127.0.0.1')"
echo "2. backend_port0 should be 5432 (PostgreSQL default port)"
echo "3. PostgreSQL must be running and accessible"
echo "4. Authentication in pool_hba.conf must allow connections"
echo "5. PostgreSQL pg_hba.conf must allow connections from pgpool"
echo ""

