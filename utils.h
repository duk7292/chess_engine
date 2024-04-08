#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <future>

#ifndef UTILS_H
#define UTILS_H

uint16_t encodeMove(const std::string &move)
{
    // Extrahieren der Start- und Zielpositionen aus der Notation
    int startFile = 'h' - move[0];
    int startRank = move[1] - '1';
    int endFile = 'h' - move[2];
    int endRank = move[3] - '1';

    int start = startRank * 8 + startFile;
    int end = endRank * 8 + endFile;

    bool isPromotion = move.length() > 4;
    int promotionType = 0; // Standardwert, falls keine Promotion

    if (isPromotion)
    {
        // Bestimmen des Promotion-Typs basierend auf dem Buchstaben
        char promotionPiece = move[6];
        switch (promotionPiece)
        {
        case 'N':
            promotionType = 0;
            break;
        case 'B':
            promotionType = 1;
            break;
        case 'R':
            promotionType = 2;
            break;
        case 'Q':
            promotionType = 3;
            break;
        }
    }

    // Zusammensetzen des Zuges in uint16_t
    uint16_t encodedMove = start;
    encodedMove |= (end << 6);
    if (isPromotion)
    {
        encodedMove |= (1 << 12);
        encodedMove |= (promotionType << 13);
    }

    return encodedMove;
}

std::string decodeMove(uint16_t move)
{
    // Extrahieren der Start- und Zielpositionen
    int start = move & 0x3F;                // Die ersten 6 Bits
    int end = (move >> 6) & 0x3F;           // Die nächsten 6 Bits
    bool isPromotion = (move >> 12) & 0x1;  // Das 13. Bit für Pawn Promotion
    int promotionType = (move >> 13) & 0x3; // Die Bits 14 und 15 für den Typ der Promotion

    // Umwandeln der Positionen in Schachnotation (h1, a8, etc.)
    std::string startFile = std::string(1, 'h' - (start % 8));
    std::string startRank = std::string(1, '1' + (start / 8));

    std::string endFile = std::string(1, 'h' - (end % 8));
    std::string endRank = std::string(1, '1' + (end / 8));
    std::string promotionPiece;
    if (isPromotion)
    {
        switch (promotionType)
        {
        case 0:
            promotionPiece = "N";
            break; // Springer
        case 1:
            promotionPiece = "B";
            break; // Läufer
        case 2:
            promotionPiece = "R";
            break; // Turm
        default:
            promotionPiece = "Q";
            break; // Dame
        }
    }

    // Zusammensetzen des Zuges
    std::string decodedMove = startFile + startRank + endFile + endRank;

    // Hinzufügen der Bauernumwandlung, falls vorhanden
    if (isPromotion)
    {
        decodedMove += "=" + promotionPiece;
    }

    return decodedMove;
}

uint16_t CNNumberToMove(int number)
{
    uint16_t move = 0;

    if (number < 4096)
    {
        move = number;
        return move;
    }

    number -= 4096;
    int color = number / 88;

    number %= 88;
    int promotion = number % 4;
    int start_y = color ? 15 : 55;
    int end_y = color ? 7 : 63;
    // normal Promotion
    if (number < 32)
    {

        int x_offset = number / 4;

        move |= ((end_y - x_offset) << 6);
        move |= (start_y - x_offset);
        move |= (1 << 12);
        move |= (promotion << 13);
        return move;
    }
    number -= 32;
    if (number < 48)
    {
        int x_offset_start = 1 + number / 8;
        int x_offset_end = x_offset_start + ((number % 8 < 4) ? 1 : -1);
        move |= ((end_y - x_offset_end) << 6);
        move |= (start_y - x_offset_start);
        move |= (1 << 12);
        move |= (promotion << 13);
        return move;
    }
    number -= 48;
    if (number < 4)
    {

        move |= ((end_y - 1) << 6);
        move |= (start_y);
        move |= (1 << 12);
        move |= (promotion << 13);
        return move;
    }
    move |= ((end_y - 6) << 6);
    move |= (start_y - 7);
    move |= (1 << 12);
    move |= (promotion << 13);
    return move;
}
int MoveToCNNumber(uint16_t move)
{
    move &= ~(1 << 15);

    int number = 0;
    int promotionNumber = 0;
    int promotion = (move >> 13) & 3;
    int isPromotion = (move >> 12) & 1;
    int start = (move & 63);
    int end = (move >> 6) & 63;
    int color;

    // Determine color based on startY (reverse of original logic)
    if (isPromotion == 0)
    {
        return (int)move;
    }
    if (start >= 48)
        color = 0; // White
    else
        color = 1; // Black
    if (start == end + (color ? 8 : -8))
    {
        int x_offset = (color ? 15 : 55) - start;
        promotionNumber = x_offset * 4;
    }
    else if ((start == (color ? 15 : 55)) | (start == (color ? 8 : 48)))
    {
        if (start == (color ? 15 : 55))
        {
            promotionNumber = 80;
        }
        else
        {
            promotionNumber = 84;
        }
    }
    else
    {
        promotionNumber += 32;
        int x_offset_start = (color ? 15 : 55) - start;
        int x_offset_end = (color ? 7 : 63) - end;

        promotionNumber += (x_offset_start - 1) * 8;

        promotionNumber += (x_offset_end > x_offset_start) ? 0 : 4;

        // std::cout << number <<" "<<x_offset_start << std::endl;
    }

    promotionNumber += promotion;

    // Adjust number based on color
    promotionNumber += color * 88;

    number = promotionNumber + 4096;

    return number; // The initial offset adjustment is no longer required
}

#endif