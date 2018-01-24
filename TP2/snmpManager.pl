use SNMP;
use threads;
use Net::WebSocket::Server;
use JSON;

# Starts SNMP session and find available partitions

my $session : shared = new SNMP::Session(DestHost => $ARGV[0], Version => '2c');
($fsIndex) = $session->bulkwalk(0, 1, [['hrPartitionFSIndex']]);

my @ids;
for $i (0..@$fsIndex-1) {
	$id = $$fsIndex[$i]->val;
	if ($id != -1) {
		push @ids, $id;
	}
}

# Subroutine to get and send the partitions' data

sub getValues {
	my $i = @_[0];
	my $session = @_[1];
	my $conn = @_[2];

	while (1) {
		{
			lock($session);
			$s = $session->get(['hrFSStorageIndex',$i]);
		}
		$vars = new SNMP::VarList(
			['hrStorageDescr',$s],
			['hrStorageSize',$s],
			['hrStorageUsed',$s],
			['hrStorageAllocationUnits',$s]);
		{
			lock($session);
			@vals = $session->get($vars);
		}
		$toGB = $vals[3] / (10 ** 9);
		$msg = JSON->new->encode({
			desc => $vals[0],
			size => sprintf("%.2f", $vals[1] * $toGB),
			used => sprintf("%.4f", $vals[2] * $toGB),
			free => sprintf("%.2f", (1 - $vals[2]/$vals[1]) * 100)
		});
		$conn->send('json', $msg);
		$time = ($free < 10) ? 1 : 3;
		$time = ($free >= 10 && $free < 30) ? 3 : 5;
		$time = ($free >= 30 && $free < 50) ? 5 : 10;
		sleep($time);
	}
}

# Starts WebSocket

Net::WebSocket::Server->new(
    listen => 8081,
    on_connect => sub {
        my ($serv, $conn) = @_;
        $conn->on(
            utf8 => sub {
                my ($conn, $msg) = @_;
                my @threads;
                my $pos = 0;
				foreach my $i (@ids) {
					push @threads, threads->create(getValues($i, $session, $conn));
				}
				foreach my $thr (@threads) {
					$thr->join();
				}
            },
        );
    },
)->start;