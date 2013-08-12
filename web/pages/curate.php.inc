<?php

include_once 'include/this_alot.php.inc';

$db = $config->mkdb();
$config->require_curator();

if ($config->is_post()) {
    $id = $config->post('id');
    $rating = $config->post('rating');

    if (!$id || !$rating) {
        $config->http_status(400, 'Missing parameters');
    } else if ($db->set_alot_rating($id, $rating)) {
        echo "Rating $rating saved for alot $id";
    } else {
        $config->http_status(404, 'Not Found');
    }

    die();
}

$page = $config->get_param('page', 0);
if ($page < 0 || !is_numeric($page)) {
    $config->error(400, 'Bad page');
}

$pageSize = 20;
$alots = $db->all_alots($page * $pageSize, $pageSize);
if ($alots === FALSE) {
    $config->error(404, 'No more alots');
}

/**
 * @param Config $config
 * @param int $page
 */
function paging_controls($config, $page) {
    $url = $config->asset_url('curate/');
    ?>
    <h4 class="paging clearfix">
        <?php if ($page > 0) {?>
            <a class="btn pull-left" href="<?php echo $url . ($page - 1) ?>">Previous page</a>
        <?php } ?>
        <a class="btn pull-right" href="<?php echo $url . ($page + 1) ?>">Next page</a>
    </h4>
<?php
}
?>
<!DOCTYPE html>
<html>
<head>
    <title>Alot of content</title>
    <link href="//netdna.bootstrapcdn.com/twitter-bootstrap/2.3.2/css/bootstrap-combined.min.css" rel="stylesheet">
    <style>
        h1 a {
            color: #333;
        }
        .paging {
            margin: 10px;
        }
    </style>
</head>
<body>
<div class="container">
    <h1><a href="<?php echo $config->asset_url('curate') ?>">Curate content</a></h1>
    <h4>A rating of 1/5 will hide an alot from the front page.</h4>

    <?php paging_controls($config, $page) ?>

    <table class="table">

        <tr>
            <th>id</th>
            <th>Date</th>
            <th>Alot of...</th>
            <th>Awesomeness</th>
        </tr>

        <?php
        foreach ($alots as $alot) {
            $id = $alot->id;
            $alot_url = $alot->url();
            $composed_url = $alot->asset_url();
            $word = $alot->safe_word();
            ?>
            <tr data-id="<?php echo $id ?>">
                <td>
                    <a target="_blank" href="<?php echo $alot_url ?>">
                        <?php echo $id ?>
                    </a>
                </td>
                <td><?php echo $alot->added ?></td>
                <td>
                    <?php
                    show_alot($alot);
                    ?>
                </td>
                <td>
                    <div class="btn-group">
                        <?php foreach(range(1, 5) as $rating) { ?>
                            <button type='button'
                                    class="btn rating-button <?php echo $alot->curator_rating == $rating ? 'active' : '' ?>"
                                    value="<?php echo $rating ?>">
                                <?php echo $rating ?>
                            </button>
                        <?php } ?>
                    </div>
                </td>
            </tr>
        <?php } ?>
    </table>
    <?php paging_controls($config, $page) ?>
</div>
<script src="//ajax.googleapis.com/ajax/libs/jquery/1.10.2/jquery.min.js"></script>
<script type="text/javascript">
    $(document).ready(function() {
        $('.btn-group .rating-button').on('click', function() {
            var btn = $(this);
            var rating = btn.val();
            var id = btn.parents('tr').data('id');

            var allbuttons = $(this).parent().find('.btn');
            allbuttons.prop('disabled', true);

            $.post('<?php echo $config->asset_url('curate') ?>', {
                id: id,
                rating: rating
            })
                .done(function() {
                    allbuttons.removeClass('active');
                    btn.addClass('active');
                })
                .error(function() {
                    alert('Error submitting rating ' + rating + ' for alot #' + id);
                })
                .always(function() {
                    allbuttons.prop('disabled', false);
                });
        });
    });
</script>
</body>
</html>