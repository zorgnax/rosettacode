#!/usr/bin/perl

sub add_edge {
    my ($g, $a, $b, $weight) = @_;
    $g->{$a} ||= {name => $a};
    $g->{$b} ||= {name => $b};
    push @{$g->{$a}{edges}}, {weight => $weight, vertex => $g->{$b}};
}

sub push_priority {
    my ($a, $v) = @_;
    push @$a, $v;
    @$a = sort @$a;
}

sub dijkstra {
    my ($g, $a, $b) = @_;
    for my $v (values %$g) {
        $v->{dist} = 9999999;
        delete $v->{prev};
        delete $v->{visited};
    }
    $g->{$a}{dist} = 0;
    my $h = [];
    push_priority($h, $g->{$a});
    while (1) {
        my $v = shift @$h;
        last if !$v || $v->{name} eq $b;
        $v->{visited} = 1;
        for my $e (@{$v->{edges}}) {
            my $u = $e->{vertex};
            if (!$u->{visited} && $v->{dist} + $e->{weight} <= $u->{dist}) {
                $u->{prev} = $v;
                $u->{dist} = $v->{dist} + $e->{weight};
                push_priority($h, $u);
            }
        }
    }
}

my $g = {};
add_edge($g, "a", "b", 7);
add_edge($g, "a", "c", 9);
add_edge($g, "a", "f", 14);
add_edge($g, "b", "c", 10);
add_edge($g, "b", "d", 15);
add_edge($g, "c", "d", 11);
add_edge($g, "c", "f", 2);
add_edge($g, "d", "e", 6);
add_edge($g, "e", "f", 9);
dijkstra($g, "a", "e");
my $v = $g->{e};
my @a;
while ($v) {
    push @a, $v->{name};
    $v = $v->{prev};
}
my $path = join "", reverse @a;
print "$g->{e}{dist} $path\n";

