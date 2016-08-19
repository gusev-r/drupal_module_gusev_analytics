<?php foreach ($test as $key => $countPeople): ?>
    <strong><?php print $countPeople->name; ?></strong>
    <?php print $countPeople->count; ?> <br>
<?php endforeach; ?>