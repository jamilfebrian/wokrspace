<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use Illuminate\Database\Eloquent\SoftDeletes;

class TransactionPassanger extends Model
{
    use HasFactory, SoftDeletes;
    protected $fillable = [
        'transaction_id',
        'flight_seat_id',
        'name',
        'birth_date',
        'nationality',
    ];

    public function transaction()
    {
        return $this->belongsTo(Transaction::class);
    }

    public function flightSeat()
    {
        return $this->belongsTo(FlightSeat::class);
    }
}
