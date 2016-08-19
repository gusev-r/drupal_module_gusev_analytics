<h1>Просмотры</h1>
<?php foreach ($pages_viewed as $key => $countPeople): ?>
    <strong><?php print $countPeople->title . '(' . $countPeople->nid . ')'; ?></strong>
    <?php print $countPeople->viewed; ?> <br>
<?php endforeach; ?>
<?php


