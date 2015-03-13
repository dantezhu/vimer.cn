<?php
/* -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
# ***** BEGIN LICENSE BLOCK *****
# This file is part of Plume Framework, a simple PHP Application Framework.
# Copyright (C) 2001-2007 Loic d'Anterroches and contributors.
#
# Plume Framework is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or
# (at your option) any later version.
#
# Plume Framework is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
# ***** END LICENSE BLOCK ***** */

class Pluf_Form_Field_Float extends Pluf_Form_Field
{
    public $widget = 'Pluf_Form_Widget_TextInput';
    public $max_value = null;
    public $min_value = null;

    public function clean($value)
    {
        parent::clean($value);
        if (in_array($value, $this->empty_values)) {
            $value = '';
        }
        if (!is_numeric($value)) {
            throw new Pluf_Form_Invalid(__('Enter a number.'));
        }
        $value = (float) $value;
        if ($this->max_value !== null and $this->max_value < $value) {
            throw new Pluf_Form_Invalid(sprintf(__('Ensure this value is less than or equal to %s.'), $this->max_value));
        }
        if ($this->min_value !== null and $this->min_value > $value) {
            throw new Pluf_Form_Invalid(sprintf(__('Ensure this value is greater than or equal to %s.'), $this->min_value));
        }
        return $value;
    }
}
